use serde::Deserialize;
use std::{fs::File, io::BufReader};
use tokio::process::Command;

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

    for test in &tests {
        let cmd = Command::new("ls");
        println!("test name: {}", test.name);
    }
}
