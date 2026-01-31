#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mutex;
int shared_counter = 0;

void increment(int iterations) {
    for(int i = 0; i < iterations; ++i) {
        ++shared_counter;
    }
}

void decrement(int iterations) {
    for(int i = 0; i < iterations; ++i) {
        --shared_counter;
    }
}

int main() {
    std::jthread t1(increment, 1000);
    std::jthread t2(decrement, 1000);
    std::cout << shared_counter << std::endl;
    t1.join();
    t2.join();
}