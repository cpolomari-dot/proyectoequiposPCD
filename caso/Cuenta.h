#pragma once
#include <condition_variable>
#include <mutex>

class Cuenta {
public:
    Cuenta() : saldo_(200) {}

    static bool transferir(Cuenta& origen, Cuenta& destino, long importe);
    long saldo() const;

private:
    mutable std::mutex m_;
    std::condition_variable cv_;
    long saldo_;
};