#pragma once

#include "Contexto.h"
#include "CuentaBancaria.h"

class Banco {
public:
    explicit Banco(const Contexto& ctx);

    bool ejecutar();

private:
    const Contexto& _ctx;
    CuentaBancaria _cuenta;
};
