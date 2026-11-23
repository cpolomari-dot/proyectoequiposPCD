#include "Cuenta.h"
#include <chrono>
#include <thread>

bool Cuenta::transferir(Cuenta& origen, Cuenta& destino, long importe) {
    if (&origen == &destino) 
    return false;

    std::scoped_lock bloqueo(origen.m_, destino.m_);

    if (origen.saldo_ < importe) 
    return false;
    origen.saldo_ -= importe;
    destino.saldo_ += importe;
    destino.cv_.notify_all();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // la operación dura 1 ms, con las cuentas cogidas
    return true;
}

long Cuenta::saldo() const {
    std::scoped_lock g(m_);
    return saldo_;
}