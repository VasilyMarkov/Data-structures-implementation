#!/bin/sh

set -e -x

echo "Installing dependencies"

apt-get update

DEBIAN_FRONTEND="noninteractive" apt-get -y install tzdata

apt-get install -y software-properties-common

add-apt-repository -y ppa:ubuntu-toolchain-r/test

apt-get install -y wget rsync

# LLVM
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
add-apt-repository -y "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-20 main"

# CMake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add -
apt-add-repository 'deb https://apt.kitware.com/ubuntu/ jammy main'

apt-get update

apt-get install -y \
	ssh \
	vim \
	make \
	cmake \
	build-essential \
	ninja-build \
	git \
	linux-tools-common \
	linux-tools-generic \
	g++-15 \
	clang-20 \
	clang-format-20 \
	clang-tidy-20 \
	libc++-20-dev \
	libc++abi-20-dev \
	libclang-rt-20-dev \
	clangd-20 \
	lldb-20 \
	gdb \
	binutils-dev \
	libdwarf-dev \
	libdw-dev \
	python3 \
	python3-pip \
	python3-venv \
	ca-certificates \
	openssh-server \
	autoconf \
	flex

git clone https://github.com/google/googletest.git -b v1.17.0 /tmp/googletest && \
    mkdir -p /tmp/googletest/build && \
    cd /tmp/googletest/build && \
    cmake .. \
        -DCMAKE_CXX_COMPILER=clang++-20 \
        -DCMAKE_C_COMPILER=clang-20 \
        -DCMAKE_CXX_STANDARD=17 \
        -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j$(nproc) && \
    make install && \
    rm -rf /tmp/googletest