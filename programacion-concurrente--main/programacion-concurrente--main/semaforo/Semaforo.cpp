
#include "Semaforo.h"

Semaforo::Semaforo(int valorInicial)
    : _valor(valorInicial) {}

void Semaforo::acquire() {
    std::unique_lock<std::mutex> lock(_m);

    _cv.wait(lock, [this]() {
        return _valor > 0;
    });

    --_valor;
}

void Semaforo::release() {
    {
        std::lock_guard<std::mutex> lock(_m);
        ++_valor;
    }

    _cv.notify_one();
}

bool Semaforo::tryAcquireFor(std::chrono::milliseconds plazo) {
    std::unique_lock<std::mutex> lock(_m);

    if (!_cv.wait_for(lock, plazo, [this]() {
            return _valor > 0;
        })) {
        return false;
    }

    --_valor;
    return true;
}
