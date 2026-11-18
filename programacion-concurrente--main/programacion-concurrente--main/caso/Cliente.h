
#pragma once

#include "Contexto.h"

class CuentaBancaria;

class Cliente {
public:
    Cliente(const Contexto& ctx,
            CuentaBancaria& cuenta,
            int id,
            int operaciones,
            long cantidad);

    void ejecutar();

private:
    const Contexto& _ctx;
    CuentaBancaria& _cuenta;
    int _id;
    int _operaciones;
    long _cantidad;
};
