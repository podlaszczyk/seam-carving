FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update -y && apt install -y \
      cmake \
      gcc g++ \
      gdb \
      clang \
      clang-format \
      ninja-build \
      vim \
      git \
      libopencv-dev
