#pragma once

#include "Contexto.h"
#include "Semaforo.h"

#include <mutex>
#include <queue>
#include <semaphore>

class Antifraude {
public:
    Antifraude();

    void encolar(int cuenta);

    int desencolar();

    void bucleHilo(const Contexto& ctx);

private:
    static constexpr int CAPACIDAD = 8;
    Semaforo _huecos;
    std::counting_semaphore<CAPACIDAD> _elementos;
    std::mutex _mutexCola;
    std::queue<int> _cola;
};
