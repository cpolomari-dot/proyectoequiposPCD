#include "Cuenta.h"

bool Cuenta::transferir(Cuenta& origen, Cuenta& destino, long importe) {
    if (&origen == &destino) return false;

    std::scoped_lock bloqueo(origen.m_, destino.m_);   // las dos a la vez

    if (origen.saldo_ < importe) 
    return false;         // comprobar y restar, en el mismo bloque
    origen.saldo_ -= importe;
    destino.saldo_ += importe;
    destino.cv_.notify_all();                          // avisa a una posible retirada
    return true;
}

long Cuenta::saldo() const {
    std::scoped_lock g(m_);
    return saldo_;
}