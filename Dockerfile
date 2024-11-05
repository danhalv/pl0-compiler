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

COPY . .

WORKDIR /pl0-compiler/build
