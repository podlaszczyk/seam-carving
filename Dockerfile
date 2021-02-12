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


ARG SSH_PORT=2323
RUN apt install -y openssh-server ssh-askpass && apt install  -y rsync grsync && \
    ssh-keygen -A && mkdir -p /run/sshd

RUN echo "Port $SSH_PORT" >> /etc/ssh/sshd_config

EXPOSE $SSH_PORT

RUN echo 'root:pass' | chpasswd


RUN apt -y install sudo
RUN useradd -m user
RUN echo 'user:user' | chpasswd && adduser user sudo
RUN echo "user ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
WORKDIR /workdir
USER user
ENTRYPOINT ["./entrypoint.sh"]

