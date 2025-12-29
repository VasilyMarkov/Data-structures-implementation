FROM ubuntu:22.04

COPY install_deps.sh /
RUN sh install_deps.sh

WORKDIR /C++

RUN useradd -m -s /bin/bash v.markov && \
    chown -R v.markov:v.markov /C++

ENV TZ=Europe/Moscow

ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

ENV CXX="/usr/bin/clang++-20"
ENV CC="/usr/bin/clang-20"


RUN chmod 755 /C++

USER v.markov
