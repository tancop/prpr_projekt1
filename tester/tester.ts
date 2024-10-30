import yaml from "js-yaml";
import { z } from "zod";

const Config = z.object({
  tests: z.array(
    z.object({
      name: z.string(),
      points: z.number(),
      files_available: z.boolean().optional(),
      sequence: z.array(z.record(z.enum(["in", "out"]), z.string())),
    }),
  ),
});

type Config = z.infer<typeof Config>;

let shouldRecompile = false;

let configText = await Bun.file("input.yaml").text();
let sourcePath = process.env.TESTER_SOURCE_PATH;
if (!sourcePath) {
  console.error("Source path not set");
  process.exit(1);
}
let sourceText = await Bun.file(sourcePath).text();

let configHashFile = Bun.file(".config-hash");
let configHash = "";
try {
  configHash = await configHashFile.text();
} catch {}

let realConfigHash = Bun.hash(configText).toString(16);

if (configHash != realConfigHash) {
  shouldRecompile = true;
  configHash = realConfigHash;
  Bun.write(configHashFile, configHash);
}

let sourceHashFile = Bun.file(".source-hash");
let sourceHash = "";
try {
  sourceHash = await sourceHashFile.text();
} catch {}

let realSourceHash = Bun.hash(sourceText).toString(16);

if (sourceHash != realSourceHash) {
  shouldRecompile = true;
  sourceHash = realSourceHash;
  Bun.write(sourceHashFile, sourceHash);
}

console.log(`should recompile: ${shouldRecompile}`);

let configResult = Config.safeParse(yaml.load(configText));

if (!configResult.success) {
  console.error("Config failed to load with error:", configResult.error);
  process.exit(2);
}

if (shouldRecompile) {
  let compileCommand = process.env.TESTER_COMPILE_COMMAND;
  if (!compileCommand) {
    console.error("Compile command not set");
    process.exit(1);
  }
  let proc = Bun.spawn(compileCommand!.split(" "), { stdout: "inherit" });
  let exitCode = await proc.exited;
  if (exitCode != 0) {
    console.error(`Compiler exited with code ${exitCode}`);
    process.exit(3);
  }
}

let config = configResult.data!;

if (!process.env.TESTER_RUN_COMMAND) {
  console.error("Run command not set");
  process.exit(1);
}

let runCommand = process.env.TESTER_RUN_COMMAND!;

for (const test of config.tests) {
  console.log("running test", test.name);
  let proc = Bun.spawn(runCommand.split(" "), {
    stdin: "pipe",
  });

  for (const line of test.sequence) {
    if (Object.hasOwn(line, "in")) {
      console.log("writing stdin");
      proc.stdin.write(`${line.in!}\n`);
    } else {
      // out
      console.log("reading stdout");

      const output = await new Response(proc.stdout).text();

      console.log(output);
      /*if (res !== line.out!) {
        console.error(
          `Test ${test.name} failed, got output: "${res}", expected: "${line.out!}"`,
        );
        }*/
    }
  }

  if (!proc.exitCode) {
    console.error(`Test ${test.name} failed to exit`);
  }

  if (proc.exitCode !== 0) {
    console.error(
      `Test ${test.name} failed, got non-zero exit code ${proc.exitCode}`,
    );
  } else {
    `Test ${test.name} success`;
  }
}
