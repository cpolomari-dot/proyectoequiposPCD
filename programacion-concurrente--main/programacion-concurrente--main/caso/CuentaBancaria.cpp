
#include "CuentaBancaria.h"

#include "Config.h"

#include <chrono>
#include <thread>

CuentaBancaria::CuentaBancaria(const Contexto& ctx, long saldoInicial)
    : _ctx(ctx),
      _saldo(saldoInicial),
      _sinCerrojo(ctx.config.fallo("carrera")) {}

void CuentaBancaria::ingresar(long cantidad) {
    if (_sinCerrojo) {
        // Simulacion de una actualizacion no sincronizada.
        long visto = _saldo.load();

        std::this_thread::sleep_for(
            std::chrono::microseconds(50));

        _saldo.store(visto + cantidad);
        return;
    }

    // Version correcta: operacion protegida.
    std::lock_guard<std::mutex> lock(_m);
    _saldo.fetch_add(cantidad);
}

long CuentaBancaria::saldo() {
    return _saldo.load();
}
