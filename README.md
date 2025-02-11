# pl0-compiler
Compiler for PL/0 programming language

## Dependencies
`Docker version 20.10.17`

Optional:\
`Python 3.12.4` (requirements.txt installs `clang-format` and `cmake-format`
commands)

## How to run
The compiler can either be run locally or in a docker container.

### Docker container
The `run` service can be used to run the compiler in a docker container.
Example:\
`FILE=../pl0-compiler/examples/sum.pl0 docker compose up run`

Another example which passes arguments to the compiler:\
`ARGS=--help docker compose up run`

Another example which prints the generated x86-64 assembly for a given pl0
program:\
`ARGS=--assembly FILE=../pl0-compiler/examples/exponentiation.pl0 docker compose
up run`

### Locally
The program can be built using docker:\
`docker compose up build`

The `build/` directory is mounted locally and can be accessed, where the
compiler binary can be found in `./build/pl0-compiler/pl0c`.\
The output binary is an x86-64 ELF executable.

## Docker services
run:\
`ARGS=<arguments> FILE=<file> docker compose up run`

build:\
`docker compose up build`

clang-format-check:\
`docker compose up clang-format-check`

cmake-format-check:\
`docker compose up cmake-format-check`

test:\
`docker compose up test`

## Development
For convenient access to `clang-format` and `cmake-format` commands:
```
python3 -m venv venv
source venv/bin/activate
pip3 install -r requirements.txt
```

## Example Programs
Some example PL/0 programs can be found under `pl0-compiler/examples/`.\
NOTE: Programs requiring user input will have input default to a value of `0` when run
in a docker container.

## References
* [PL/0 Wiki](https://en.wikipedia.org/wiki/PL/0)
* [Lexical specification](https://courses.cs.washington.edu/courses/cse401/02sp/pl0/lex_orig.html)
* [Syntax specification](https://courses.cs.washington.edu/courses/cse401/02sp/pl0/bnf_orig.html)
* [Additional information](https://courses.cs.washington.edu/courses/cse401/02sp/pl0/proj.html)
