#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

class Estadisticas;

// Clase Cuenta: monitor que encapsula el saldo, el mutex y la variable de condición.
// Cada cuenta protege su propio estado cumpliendo con RAII.
class Cuenta {
public:
    Cuenta() : saldo_(200) {}
    explicit Cuenta(long saldo_inicial) : saldo_(saldo_inicial) {}

    // P1 / P5: Transferencia segura entre dos cuentas cogiendo ambos mutex a la vez.
    // El parámetro carrera permite simular el modo de fallo sin sincronización.
    static bool transferir(Cuenta& origen, Cuenta& destino, long importe, bool carrera = false);

    // P2: Ingreso de saldo en la cuenta. Despierta a los hilos en espera en cv_.
    void ingresar(long importe);

    // P2: Retirada de saldo con espera en la variable de condición (máximo 20 ms).
    // Si no hay saldo suficiente, espera hasta que llegue dinero o venza el plazo.
    bool retirar(long importe,
                 std::chrono::milliseconds timeout = std::chrono::milliseconds(20),
                 Estadisticas* estadisticas = nullptr);

    // Consulta de saldo actual (segura con cerrojo).
    long saldo() const;

private:
    mutable std::mutex m_;
    std::condition_variable cv_;
    long saldo_;
};
