#include "Cuenta.h"
#include "Estadisticas.h"

#include <chrono>
#include <thread>

bool Cuenta::transferir(Cuenta& origen, Cuenta& destino, long importe, bool carrera) {
    if (&origen == &destino) {
        return false;
    }

    // MODO DE FALLO: CARRERA DE DATOS (P5)
    if (carrera) {
        if (origen.saldo_ < importe) {
            return false;
        }
        origen.saldo_ -= importe;
        destino.saldo_ += importe;
        destino.cv_.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return true;
    }

    // VERSIÓN CORRECTA CON RAII (P1)
    std::scoped_lock bloqueo(origen.m_, destino.m_);

    if (origen.saldo_ < importe) {
        return false;
    }

    origen.saldo_ -= importe;
    destino.saldo_ += importe;

    // P2: Avisos desde las transferencias que llegan a la cuenta
    destino.cv_.notify_all();

    // Cada operación de cliente dura 1 ms, con sus cuentas cogidas
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return true;
}

void Cuenta::ingresar(long importe) {
    std::unique_lock<std::mutex> lk(m_);
    saldo_ += importe;

    // P2: Ingresos que avisan a los hilos esperando saldo en esta cuenta
    cv_.notify_all();

    // Cada operación de cliente dura 1 ms, con sus cuentas cogidas
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

bool Cuenta::retirar(long importe, std::chrono::milliseconds timeout, Estadisticas* estadisticas) {
    std::unique_lock<std::mutex> lk(m_);

    // P2: Retirada con espera de saldo (máx. 20 ms).
    // Si no hay saldo, espera a que llegue dinero a esa cuenta.
    if (saldo_ < importe) {
        auto inicio = std::chrono::steady_clock::now();
        cv_.wait_for(lk, timeout, [this, importe]() {
            return saldo_ >= importe;
        });
        auto duracion = std::chrono::steady_clock::now() - inicio;
        if (estadisticas) {
            estadisticas->registrarEspera("cliente", duracion);
        }

        // Si tras el tiempo máximo no llegó dinero suficiente, no se hace.
        if (saldo_ < importe) {
            return false;
        }
    }

    saldo_ -= importe;

    // Cada operación de cliente dura 1 ms, con sus cuentas cogidas
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return true;
}

long Cuenta::saldo() const {
    std::scoped_lock g(m_);
    return saldo_;
}
