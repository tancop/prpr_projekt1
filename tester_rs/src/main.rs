use core::str;
use serde::Deserialize;
use std::{
    env,
    fs::File,
    io::BufReader,
    process::{ExitStatus, Stdio},
    time::Duration,
};
use tokio::{
    io::{AsyncBufReadExt, AsyncWriteExt},
    process::Command,
};

#[derive(Deserialize)]
struct SequenceEntry {
    #[serde(rename = "in")]
    input: Option<String>,
    #[serde(rename = "out")]
    output: Option<String>,
}

#[derive(Deserialize)]
struct Test {
    name: String,
    points: f32,
    sequence: Vec<SequenceEntry>,
}

#[derive(Deserialize)]
struct InputFile {
    tests: Vec<Test>,
}

#[tokio::main]
async fn main() {
    _ = dotenvy::dotenv();

    let Ok(input_file) = File::open("input.yaml") else {
        println!("error: input.yaml not found");
        return;
    };

    let Ok(yaml) = serde_yml::from_reader::<BufReader<File>, InputFile>(BufReader::new(input_file))
    else {
        println!("error: invalid input.yaml");
        return;
    };

    let tests = yaml.tests;

    let Ok(run_command) = env::var("TESTER_RUN_COMMAND") else {
        println!("error: run command not set");
        return;
    };

    let mut success_count = 0;
    let tests_count = tests.len();

    for test in tests {
        let mut cmd = Command::new(&run_command);
        cmd.stdout(Stdio::piped());
        cmd.stdin(Stdio::piped());

        let Ok(mut child) = cmd.spawn() else {
            println!("test {}: failed to spawn", test.name);
            break;
        };

        let Some(stdout) = child.stdout.take() else {
            println!("test {}: failed to open stdout", test.name);
            break;
        };

        let Some(mut stdin) = child.stdin.take() else {
            println!("test {}: failed to open stdin", test.name);
            break;
        };

        let mut reader = tokio::io::BufReader::new(stdout);

        let test_name = test.name.clone();
        tokio::spawn(async move {
            if let Ok(status) = child.wait().await {
                if !status.success() {
                    if let Some(code) = status.code() {
                        println!("test {} failed with exit code {}", test_name, code);
                    }
                }
            }
        });

        let mut buf = Vec::new();

        const LINE_READ_TIMEOUT_MS: u64 = 10;

        let mut success = true;

        'seq: for i in 0..test.sequence.len() {
            let entry = &test.sequence[i];
            if let Some(input) = &entry.input {
                let mut input_line = input.clone();
                input_line.push('\n');
                match stdin.write(input_line.as_bytes()).await {
                    Ok(_) => (),
                    Err(err) => {
                        println!("test \"{}\" failed to accept stdin: {}", test.name, err);
                        success = false;
                    }
                };
            } else if let Some(output) = &entry.output {
                while let Ok(_) = tokio::time::timeout(
                    Duration::from_millis(LINE_READ_TIMEOUT_MS),
                    reader.read_until(b'\n', &mut buf),
                )
                .await
                {
                    let Ok(buf_string) = str::from_utf8(&buf) else {
                        println!("test \"{}\" failed: output is not valid UTF-8", test.name);
                        success = false;
                        break 'seq;
                    };
                    if buf_string != output {
                        println!(
                            "test \"{}\" failed: expected line \"{}\", found \"{}\"",
                            test.name, output, buf_string
                        );
                        success = false;
                        break 'seq;
                    }
                }
            }
        }
        drop(stdin);

        if success {
            success_count += 1;
            println!("test \"{}\" success", test.name);
        }
    }

    println!("----------------");
    println!(
        "total: {} failed, {} success",
        tests_count - success_count,
        success_count
    );
}
