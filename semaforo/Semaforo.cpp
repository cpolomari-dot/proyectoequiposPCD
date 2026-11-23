#include "Semaforo.h"

// TODO: implementar. Ahora mismo compila, pero no sincroniza nada.

Semaforo::Semaforo(int valorInicial) : _valor(valorInicial) {}


void Semaforo::acquire() {

    // Creamos un bloqueo para proteger el valor del semaforo.
    // Asi evitamos que varios hilos lo modifiquen a la vez.
    std::unique_lock<std::mutex> lock(_m);

    // Comprobamos si hay permisos disponibles.
    // Si el valor es 0, el hilo se queda esperando.
    // Mientras espera, libera el mutex para que otro hilo
    // pueda devolver un permiso mediante release().
    _cv.wait(lock, [this]() {

        // Solo podemos continuar si queda algun permiso.
        return _valor > 0;
        });

    // Cuando conseguimos entrar, consumimos un permiso.
    // Por ejemplo, si habia 3 permisos, ahora quedan 2.
    --_valor;

    // Al terminar la funcion, el bloqueo se libera solo.
}


void Semaforo::release() {
    // TODO
}

bool Semaforo::tryAcquireFor(std::chrono::milliseconds plazo) {
    (void)plazo;  // TODO
    return false;
}
