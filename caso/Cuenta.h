#pragma once
#include <condition_variable>
#include <mutex>

class Cuenta {
public:
    Cuenta() : saldo_(200) {}

    static bool transferir(Cuenta& origen, Cuenta& destino, long importe);
    long saldo() const;
    int id() const { return id_; }

private:
    const int id_;
    mutable std::mutex m_;                     // protege saldo_
    std::condition_variable cv_;               // "ha llegado dinero" (la usa la retirada de P2)
    long saldo_;
};