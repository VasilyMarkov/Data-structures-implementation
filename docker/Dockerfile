FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV CC=/usr/bin/clang-20
ENV CXX=/usr/bin/clang++-20

RUN apt-get update && apt-get install -y \
    wget \
    gnupg \
    software-properties-common \
    lsb-release \
    && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-20 main" \
    && apt-get update

RUN apt-get install -y \
    clang-20 \
    clang-tidy-20 \
    clang-format-20 \
    clangd-20 \
    lldb-20 \
    lld-20 \
    libc++-20-dev \
    libc++abi-20-dev \
    libunwind-20-dev \
    cmake \
    ninja-build \
    pkg-config \
    git \
    curl \
    wget \
    tar \
    xz-utils \
    gdb \
    valgrind \
    libboost-all-dev \
    libgtest-dev \
    python3 \
    python3-pip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --no-cache-dir conan

RUN pip3 install --no-cache-dir \
    cpplint \
    cppcheck \
    bear \
    compdb

WORKDIR /tmp
RUN git clone https://github.com/google/googletest.git \
    && cd googletest \
    && cmake -B build -G Ninja \
    -DCMAKE_C_COMPILER=clang-20 \
    -DCMAKE_CXX_COMPILER=clang++-20 \
    -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
    -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++ -fuse-ld=lld" \
    -DCMAKE_SHARED_LINKER_FLAGS="-stdlib=libc++ -fuse-ld=lld" \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -Dgtest_force_shared_crt=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build build --parallel $(nproc) \
    && cmake --install build \
    && cd .. \
    && rm -rf googletest

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-20 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-20 100 \
    && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-20 100 \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-20 100 \
    && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-20 100 \
    && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-20 100 \
    && update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-20 100

RUN git config --global init.defaultBranch main \
    && git config --global --add safe.directory '*'


WORKDIR /C++

RUN useradd -m -s /bin/bash v.markov && \
    chown -R v.markov:v.markov /C++

USER v.markov

CMD ["/bin/bash"]