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

    // Creamos un bloque para proteger el valor del semaforo.
    // Solo un hilo podra modificarlo a la vez.
    {
        // Bloqueamos el mutex mientras cambiamos el valor.
        std::lock_guard<std::mutex> lock(_m);

        // Devolvemos un permiso al semaforo.
        // Por ejemplo, si habia 0 permisos, ahora hay 1.
        ++_valor;

    } // Aqui se libera automaticamente el mutex.

    // Avisamos a uno de los hilos que este esperando.
    // Ese hilo podra comprobar si hay un permiso disponible.
    _cv.notify_one();
}



bool Semaforo::tryAcquireFor(std::chrono::milliseconds plazo) {

    // Bloqueamos el mutex para acceder al valor del semaforo
    // de forma segura, evitando problemas entre varios hilos.
    std::unique_lock<std::mutex> lock(_m);

    // Esperamos hasta que haya un permiso disponible
    // o hasta que se termine el tiempo indicado en "plazo".
    if (!_cv.wait_for(lock, plazo, [this]() {

        // Esta condicion comprueba si quedan permisos.
        // Si el valor es mayor que 0, podemos continuar.
        return _valor > 0;

        })) {

        // Si termina el tiempo y no hay permisos,
        // devolvemos false para indicar que no hemos entrado.
        return false;
    }

    // Si hemos llegado aqui, significa que hay un permiso.
    // Consumimos uno restando 1 al valor del semaforo.
    --_valor;

    // Devolvemos true porque hemos conseguido el permiso.
    // Al salir, el mutex se libera automaticamente.
    return true;
}
