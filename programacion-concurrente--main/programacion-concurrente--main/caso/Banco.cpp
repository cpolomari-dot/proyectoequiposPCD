
#include "Banco.h"

#include "Cliente.h"
#include "Config.h"
#include "Estadisticas.h"

#include <string>
#include <thread>
#include <vector>

Banco::Banco(const Contexto& ctx)
    : _ctx(ctx),
      _cuenta(ctx, 0) {}

bool Banco::ejecutar() {
    int clientes = _ctx.config.entero("clientes", 8);
    int operaciones = _ctx.config.entero("operaciones", 5000);
    int cantidad = _ctx.config.entero("cantidad", 1);

    if (clientes < 1 || operaciones < 1 || cantidad < 1) {
        _ctx.estadisticas.violacion(
            "Parametros de simulacion no validos");
        return false;
    }

    std::vector<std::thread> trabajadores;

    for (int i = 0; i < clientes; ++i) {
        trabajadores.emplace_back([this, i, operaciones, cantidad]() {
            Cliente cliente(
                _ctx,
                _cuenta,
                i,
                operaciones,
                cantidad);

            cliente.ejecutar();
        });
    }

    for (auto& hilo : trabajadores) {
        hilo.join();
    }

    long esperado =
        static_cast<long>(clientes) * operaciones * cantidad;

    long obtenido = _cuenta.saldo();

    _ctx.estadisticas.sumar("saldo_final", obtenido);

    if (obtenido != esperado) {
        _ctx.estadisticas.violacion(
            "Saldo final = " + std::to_string(obtenido) +
            ", esperado = " + std::to_string(esperado));
    }

    return _ctx.estadisticas.sinViolaciones();
}
