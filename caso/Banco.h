#pragma once

#include <vector>
#include "Cuenta.h"

class Estadisticas;
class Vigilante;

class Banco {
public:
    Banco(int cuentas, int distancia, int operaciones, int umbral,
          Estadisticas* estadisticas = nullptr,
          Vigilante* vigilante = nullptr);

    void setContexto(Estadisticas* estadisticas, Vigilante* vigilante);

    void cliente(int i);      // el hilo del cliente i
    long total() const;       // suma de saldos

    Cuenta& cuenta(int idx) { return cuentas_.at(idx); }
    const Cuenta& cuenta(int idx) const { return cuentas_.at(idx); }
    int numCuentas() const { return static_cast<int>(cuentas_.size()); }

private:
    std::vector<Cuenta> cuentas_;
    int distancia_;
    int operaciones_;
    int umbral_;
    Estadisticas* estadisticas_ = nullptr;
    Vigilante* vigilante_ = nullptr;
};
