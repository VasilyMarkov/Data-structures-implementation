FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

ENV CC=/usr/bin/clang-20
ENV CXX=/usr/bin/clang++-20
ENV LD=/usr/bin/lld-20

RUN apt-get update && apt-get install -y --no-install-recommends \
    wget \
    ca-certificates \
    lsb-release \
    software-properties-common \
    gnupg 

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-20 main" \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    clang-20 \
    clangd-20 \
    clang-tidy-20 \
    clang-format-20 \
    lld-20 \
    lldb-20 \
    llvm-20 \
    llvm-20-dev \
    llvm-20-runtime \
    libc++-20-dev \
    libc++abi-20-dev \
    libclang-20-dev \
    libclang-rt-20-dev \
    libunwind-20-dev \
    libomp-20-dev 

RUN apt-get install -y \
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

RUN apt-get update && apt-get install -y --no-install-recommends \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - > /etc/apt/trusted.gpg.d/kitware.gpg \
    && apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" \
    && apt-get update \
    && apt-get install -y --no-install-recommends cmake \
    && rm -rf /var/lib/apt/lists/*

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

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-20 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-20 100 \
    && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-20 100 \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-20 100 \
    && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-20 100 \
    && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-20 100 \
    && update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-20 100

RUN git config --global init.defaultBranch master \
    && git config --global --add safe.directory '*'


WORKDIR /C++

RUN useradd -m -s /bin/bash v.markov && \
    chown -R v.markov:v.markov /C++

USER v.markov

CMD ["/bin/bash"]