
#include "Cliente.h"

#include "CuentaBancaria.h"
#include "Estadisticas.h"
#include "Traza.h"
#include "Vigilante.h"

#include <string>
#include <thread>
#include <chrono>

Cliente::Cliente(const Contexto& ctx,
                 CuentaBancaria& cuenta,
                 int id,
                 int operaciones,
                 long cantidad)
    : _ctx(ctx),
      _cuenta(cuenta),
      _id(id),
      _operaciones(operaciones),
      _cantidad(cantidad) {}

void Cliente::ejecutar() {
    for (int i = 0; i < _operaciones; ++i) {
        _cuenta.ingresar(_cantidad);

        _ctx.vigilante.progreso();

        std::this_thread::sleep_for(
            std::chrono::microseconds(10));
    }

    _ctx.estadisticas.sumar("ingresos", _operaciones);

    _ctx.traza.evento(
        "cliente " + std::to_string(_id),
        "termina sus operaciones");
}
