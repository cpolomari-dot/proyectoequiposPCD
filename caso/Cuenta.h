#pragma once
#include <condition_variable>
#include <mutex>

class Estadisticas;//nombro otra clase para poder usarla en la declaracion de la clase Cuenta

//LINIAS CON ESPECIAL ATENCION PARA INTERACTUAR CON ELLAS:L13, L22
class Cuenta {
public:
    // le damos el valor que pide el enunciado a la cuenta 
    Cuenta() : saldo_(200) {}

    // Mueve importe devuelve true si se hizo y false si no había saldo.MUY IMPORTANTE si sinCerrojo es true no se cogen los mutex de las cuentas y se hace la operación sin protección activando modo carrera. Si es false se cogen los mutex de las cuentas y se hace la operación con protección.
    static bool transferir(Cuenta& origen, Cuenta& destino, long importe,
                           Estadisticas& estadisticas, bool sinCerrojo);

    // Devuelve el saldo cogiendo el mutex para evitar data races.
    long saldo() const;

    // parte para el auditor (p3) y para poder comprobar invariantes (p4), nos permite coger el mutex leer el saldo y devolver sin tener quecoger el mutex resulta muy util para no tener que bloquear otros hilos mientras se comprueba
    std::mutex& mutex() { return m_; }
    // MUY IMPORTANTE solo es seguro si quien lo llama ya tiene el mutex de esta cuenta.
    long saldoSinBloquear() const { return saldo_; }

private:
    // Comprueba el saldo se espera 1 ms y mueve el dinero. Supone que los mutex ya están cogidos menos cuando activa modo carrera.
    static bool mover(Cuenta& origen, Cuenta& destino, long importe, Estadisticas& estadisticas);

    mutable std::mutex m_;            // protege saldo_ para poder cogerlo en saldo()
    std::condition_variable cv_;      // aviso de ha llegado dinero
    long saldo_;                      // euros de la cuenta
};