FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

#Install base packages
RUN apt-get update && apt-get install -y --no-install-recommends \
    wget \
    ca-certificates \
    lsb-release \
    software-properties-common \
    gnupg \
    ninja-build \
    pkg-config \
    git \
    curl \
    tar \
    xz-utils \
    libgtest-dev \
    python3 \
    python3-pip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

ARG LLVM_VERSION=20

#Install cland toolchain
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-${LLVM_VERSION} main" \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    clang-${LLVM_VERSION} \
    clangd-${LLVM_VERSION} \
    clang-tidy-${LLVM_VERSION} \
    clang-format-${LLVM_VERSION} \
    lld-${LLVM_VERSION} \
    lldb-${LLVM_VERSION} \
    llvm-${LLVM_VERSION} \
    llvm-${LLVM_VERSION}-dev \
    llvm-${LLVM_VERSION}-runtime \
    libc++-${LLVM_VERSION}-dev \
    libc++abi-${LLVM_VERSION}-dev \
    libclang-${LLVM_VERSION}-dev \
    libclang-rt-${LLVM_VERSION}-dev \
    libunwind-${LLVM_VERSION}-dev \
    libomp-${LLVM_VERSION}-dev 

#Update alternatives links
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-${LLVM_VERSION} 100

ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
ENV LD=/usr/bin/lld

#Install the latest cmake
RUN apt-get update && apt-get install -y --no-install-recommends \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - > /etc/apt/trusted.gpg.d/kitware.gpg \
    && apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" \
    && apt-get update \
    && apt-get install -y --no-install-recommends cmake \
    && rm -rf /var/lib/apt/lists/*

#Install google tests
RUN git clone --depth 1 --branch v1.14.0 https://github.com/google/googletest.git \
    && cd googletest \
    && cmake -B build -G Ninja \
    -DCMAKE_C_COMPILER=${CC} \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
    -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++ -fuse-ld=lld -lc++ -lc++abi" \
    -DCMAKE_SHARED_LINKER_FLAGS="-stdlib=libc++ -fuse-ld=lld -lc++ -lc++abi" \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -Dgtest_force_shared_crt=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build build --parallel $(nproc) \
    && cmake --install build \
    && cd .. \
    && rm -rf googletest


#Configure git
RUN git config --global init.defaultBranch master &&\
    git config --global --add safe.directory '*' &&\
    git config --global user.name "VasilyMarkov" &&\
    git config --global user.email "mvv1993vasya@gmail.com"


WORKDIR /projects

CMD ["/bin/bash"]