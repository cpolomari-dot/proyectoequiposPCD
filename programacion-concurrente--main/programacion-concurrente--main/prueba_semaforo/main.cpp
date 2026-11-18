
#include "Semaforo.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // Test 1: Semaphore with initial value 0
    Semaforo semaforo(0);

    if (semaforo.tryAcquireFor(std::chrono::milliseconds(50))) {
        std::cout << "ERROR: Semaphore should be empty.\n";
        return 1;
    }

    // Test 2: Release and acquire
    semaforo.release();

    if (!semaforo.tryAcquireFor(std::chrono::milliseconds(50))) {
        std::cout << "ERROR: Semaphore should allow acquire.\n";
        return 1;
    }

    // Test 3: Maximum 3 threads inside simultaneously
    Semaforo limite(3);

    std::atomic<int> dentro{0};
    std::atomic<int> maximo{0};
    std::atomic<bool> error{false};

    std::vector<std::thread> hilos;

    for (int i = 0; i < 12; ++i) {
        hilos.emplace_back([&]() {
            for (int j = 0; j < 20; ++j) {
                limite.acquire();

                int actual = ++dentro;

                int anterior = maximo.load();
                while (actual > anterior &&
                       !maximo.compare_exchange_weak(anterior, actual)) {
                }

                if (actual > 3) {
                    error = true;
                }

                std::this_thread::sleep_for(
                    std::chrono::microseconds(300));

                --dentro;
                limite.release();
            }
        });
    }

    for (auto& hilo : hilos) {
        hilo.join();
    }

    if (error || dentro != 0 || maximo > 3) {
        std::cout << "ERROR: Semaphore test failed.\n";
        return 1;
    }

    std::cout << "OK\n";
    return 0;
}
