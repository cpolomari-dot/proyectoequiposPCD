
#pragma once

#include "Contexto.h"

#include <atomic>
#include <mutex>

class CuentaBancaria {
public:
    CuentaBancaria(const Contexto& ctx, long saldoInicial);

    void ingresar(long cantidad);
    long saldo();

private:
    const Contexto& _ctx;
    std::mutex _m;
    std::atomic<long> _saldo;
    bool _sinCerrojo;
};
