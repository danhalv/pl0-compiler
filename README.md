# pl0-compiler
Compiler for PL/0 programming language

## Dependencies
`Docker version 20.10.17`

Optional:\
`Python 3.12.4` (requirements.txt installs `clang-format` and `cmake-format`
commands)

## How to run
Hello world example:\
`docker-compose up --build run`

clang-format-check:\
`docker-compose up --build clang-format-check`

cmake-format-check:\
`docker-compose up --build cmake-format-check`

## Development
For convenient access to `clang-format` and `cmake-format` commands:
```
python3 -m venv venv
source venv/bin/activate
pip3 install -r requirements.txt
```
