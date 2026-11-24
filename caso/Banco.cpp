#include "Banco.h"
#include "Estadisticas.h"
#include "Vigilante.h"

Banco::Banco(int cuentas, int distancia, int operaciones, int umbral,
             Estadisticas* estadisticas, Vigilante* vigilante)
    : cuentas_(cuentas), distancia_(distancia),
      operaciones_(operaciones), umbral_(umbral),
      estadisticas_(estadisticas), vigilante_(vigilante) {}

void Banco::setContexto(Estadisticas* estadisticas, Vigilante* vigilante) {
    estadisticas_ = estadisticas;
    vigilante_ = vigilante;
}

void Banco::cliente(int i) {
    const int n = static_cast<int>(cuentas_.size());
    const int a = i % n;
    const int b = (i + distancia_) % n;

    for (int j = 0; j < operaciones_; ++j) {
        const int origen  = (j % 2 == 0) ? a : b;
        const int destino = (j % 2 == 0) ? b : a;
        const int tipoOp  = j % 5;

        if (tipoOp <= 2) {
            // tipos 0, 1, 2: transferencia de 10 + (13i + 7j) mod 100 euros del origen al destino
            const long importe = 10 + (13L * i + 7L * j) % 100;
            const bool hecha = Cuenta::transferir(cuentas_[origen], cuentas_[destino], importe);
            if (hecha && importe >= umbral_) {
                // TODO (P4): poner la cuenta origen en la cola antifraude
            }
        } else if (tipoOp == 3) {
            // tipo 3: ingreso de 30 euros en el origen
            cuentas_[origen].ingresar(30);
        } else if (tipoOp == 4) {
            // tipo 4: retirada de 60 euros del origen. Si no hay saldo, espera a que llegue dinero (máx. 20 ms)
            cuentas_[origen].retirar(60, std::chrono::milliseconds(20), estadisticas_);
        }

        // Progreso de cada operación y métrica
        if (vigilante_) {
            vigilante_->progreso();
        }
        if (estadisticas_) {
            estadisticas_->sumar("operaciones hechas", 1);
        }
    }
}

long Banco::total() const {
    long suma = 0;
    for (const Cuenta& c : cuentas_) {
        suma += c.saldo();
    }
    return suma;
}
