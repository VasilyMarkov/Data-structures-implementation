#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <queue>
#include <random>
#include "details.hpp"


std::vector<int> makeRandomVector(size_t size) {
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<int> dist {1, static_cast<int>(size)};

    auto random_gen = [&](){return dist(mersenne_engine);};
    std::vector<int> ret(size);
    std::generate(std::begin(ret), std::end(ret), random_gen);
    return ret;
}

int main() {
    auto random_vector = makeRandomVector(10);

    details::print(random_vector);
}