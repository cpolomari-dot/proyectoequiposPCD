#include "Cuenta.h"
#include <chrono>
#include <thread>


bool Cuenta::transferir(Cuenta& origen, Cuenta& destino,
    long importe, bool carrera,
    bool interbloqueo) {

    // Evitamos transferir dinero a la misma cuenta.
    if (&origen == &destino)
        return false;

    // MODO DE FALLO: CARRERA DE DATOS.
    // Realizamos la transferencia sin proteger los saldos.
    if (carrera) {

        // Comprobamos si hay suficiente dinero.
        if (origen.saldo_ < importe)
            return false;

        // Modificamos los saldos sin utilizar mutex.
        origen.saldo_ -= importe;
        destino.saldo_ += importe;

        // Avisamos a los hilos que estan esperando.
        destino.cv_.notify_all();

        // Simulamos la duracion de la transferencia.
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1));

        return true;
    }

    // VERSION CORRECTA DE LA TRANSFERENCIA.
    // Protegemos las dos cuentas mediante sus mutex.

    // MODO DE FALLO: INTERBLOQUEO.
    // Bloqueamos las cuentas por separado para provocar
    // una posible espera circular entre los hilos.
    if (interbloqueo) {

        // Bloqueamos primero la cuenta de origen.
        std::unique_lock<std::mutex> bloqueoOrigen(origen.m_);

        // Introducimos una pequena espera para facilitar
        // que otro hilo bloquee la cuenta de destino.
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1));

        // Intentamos bloquear la cuenta de destino.
        // Aqui puede producirse el interbloqueo.
        std::unique_lock<std::mutex> bloqueoDestino(destino.m_);

        // Comprobamos que exista saldo suficiente.
        if (origen.saldo_ < importe)
            return false;

        // Realizamos la transferencia.
        origen.saldo_ -= importe;
        destino.saldo_ += importe;

        // Avisamos a los hilos que esperan saldo.
        destino.cv_.notify_all();

        return true;
    }

    std::scoped_lock bloqueo(origen.m_, destino.m_);

    // Comprobamos que haya saldo suficiente.
    if (origen.saldo_ < importe)
        return false;

    // Actualizamos los saldos de forma segura.
    origen.saldo_ -= importe;
    destino.saldo_ += importe;

    // Notificamos que la cuenta destino tiene mas dinero.
    destino.cv_.notify_all();

    // La transferencia dura 1 milisegundo.
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1));

    return true;
}


long Cuenta::saldo() const {
    std::scoped_lock g(m_);
    return saldo_;
}