#include "Banco.h"

Banco::Banco(int cuentas, int distancia, int operaciones,
    int umbral, bool carrera, bool interbloqueo)
    : cuentas_(cuentas),
    distancia_(distancia),
    operaciones_(operaciones),
    umbral_(umbral),
    carrera_(carrera),
    interbloqueo_(interbloqueo) {
}

void Banco::cliente(int i) {
    const int n = static_cast<int>(cuentas_.size());
    const int a = i % n;
    const int b = (i + distancia_) % n;

    for (int j = 0; j < operaciones_; ++j) {
        const int origen  = (j % 2 == 0) ? a : b;
        const int destino = (j % 2 == 0) ? b : a;

        if (j % 5 <= 2) {                                   // tipos 0, 1, 2: transferencia
            const long importe = 10 + (13L * i + 7L * j) % 100;
            // Realizamos la transferencia utilizando el modo
// de funcionamiento seleccionado en la configuracion.
            // Ejecutamos la transferencia con el modo
// de fallo seleccionado por el usuario.
            const bool hecha = Cuenta::transferir(
                cuentas_[origen],
                cuentas_[destino],
                importe,
                carrera_,
                interbloqueo_
            );
            if (hecha && importe >= umbral_) {
                // TODO (P4): poner la cuenta origen en la cola antifraude
            }
        }
        // TODO (P2): j % 5 == 3 es un ingreso de 30, y j % 5 == 4 una retirada de 60
        // TODO: Vigilante::progreso() y la métrica "operaciones hechas"
    }
}

long Banco::total() const {
    long suma = 0;
    for (const Cuenta& c : cuentas_) suma += c.saldo();
    return suma;
}