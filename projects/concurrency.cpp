#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <queue>
#include <random>
#include "details.hpp"
#include <numeric>

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
    auto random_vector = makeRandomVector(1e8);
    
    auto mean_proc = [](const std::vector<int>& vec) {
        return std::accumulate(std::begin(vec), std::end(vec), 0.0) / vec.size();
    };

    auto res = details::time_execution(mean_proc, random_vector);

    std::cout << res << std::endl;
}