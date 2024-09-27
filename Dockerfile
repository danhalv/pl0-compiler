FROM ubuntu:latest

# install dependencies
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install build-essential -y && \
    apt-get install clang-18 clang-format-18 clang-tidy-18 -y && \
    apt-get install cmake cmake-format -y

WORKDIR /pl0-compiler

COPY . .

# build pl0-compiler
RUN mkdir -p /pl0-compiler/build
WORKDIR /pl0-compiler/build
RUN cmake ../
