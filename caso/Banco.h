#pragma once
#include <vector>
#include "Cuenta.h"

class Banco {
public:
    Banco(int cuentas, int distancia, int operaciones, int umbral);

    void cliente(int i);      // el hilo del cliente i
    long total() const;       // suma de saldos (provisional, para probar)

private:
    std::vector<Cuenta> cuentas_;
    int distancia_;
    int operaciones_;
    int umbral_;
};