# Dust Programming Language
<p>
  <img src="https://img.shields.io/badge/license-MIT-blue.svg">
  <img src="https://img.shields.io/badge/version-0.0.1-yellow">
</p>
Dust is a simple programming language interpreted in C.

## Building
Prerequisites
- MinGW binaries in PATH
- Python 3.6+
Run `build.py` Python script to build the project.
If you don't want to use Python script to build you can use this command line in the Dust directory:
```
gcc -o dust src/cli.c src/u8string.c src/tokenizer.c -I./include/
```

## Usage
You can see all commands by running `dust help` in the command line.

## License
[MIT](LICENSE) © Kadir Aksoy