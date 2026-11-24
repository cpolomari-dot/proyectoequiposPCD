#pragma once
#include <condition_variable>
#include <mutex>

class Cuenta {
public:
    Cuenta() : saldo_(200) {}

    // Realiza una transferencia entre dos cuentas.
 // Por defecto, la transferencia utiliza sincronizacion.
 // El parametro carrera permite activar el fallo experimental.
    static bool transferir(Cuenta& origen, Cuenta& destino,
        long importe, bool carrera = false);
    long saldo() const;

private:
    mutable std::mutex m_;
    std::condition_variable cv_;
    long saldo_;
};