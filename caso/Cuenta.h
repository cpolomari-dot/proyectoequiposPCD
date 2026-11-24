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
        long importe, bool carrera = false,
        bool interbloqueo = false);
    void ingresar(long importe);
    long saldo() const;
    // Permite al auditor bloquear el mutex de la cuenta.
    std::mutex& mutex() {
        return m_;
    }

    // Permite consultar el saldo cuando el auditor
    // ya tiene bloqueado el mutex de la cuenta.
    long saldoSinBloquear() const {
        return saldo_;
    }

private:
    mutable std::mutex m_;
    std::condition_variable cv_;
    long saldo_;
};