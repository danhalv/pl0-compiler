FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

# install dependencies
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install build-essential -y && \
    apt-get install clang-15 clang-format-15 clang-tidy-15 -y && \
    apt-get install libgtest-dev -y && \
    apt-get install cmake cmake-format -y

WORKDIR /pl0-compiler

RUN mkdir -p build/ \
    pl0-compiler/cmake \
    pl0-compiler/lexer \
    pl0-compiler/parser \
    pl0-compiler/codegen/

RUN touch pl0-compiler/lexer/CMakeLists.txt \
    pl0-compiler/parser/CMakeLists.txt \
    pl0-compiler/codegen/CMakeLists.txt

COPY CMakeLists.txt .clang-format .clang-tidy .
COPY cmake cmake/
COPY pl0-compiler/CMakeLists.txt pl0-compiler/CMakeLists.txt
COPY pl0-compiler/lexer pl0-compiler/lexer/
RUN cd build && cmake ../ && make lexer

COPY pl0-compiler/parser pl0-compiler/parser/
RUN cd build && cmake ../ && make parser

COPY pl0-compiler/codegen pl0-compiler/codegen

# build pl0-compiler
WORKDIR /pl0-compiler/build
RUN cmake ../
