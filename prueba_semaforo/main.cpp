
#include "Semaforo.h"
#include <chrono>
#include <iostream>

int main() {

    // PRUEBA 1: Semaforo sin permisos.

    // Creamos un semaforo que empieza con 0 permisos.
    // Esto significa que ningun hilo puede entrar.
    Semaforo semaforo(0);

    // Intentamos conseguir un permiso durante 50 milisegundos.
    // Como el semaforo tiene 0 permisos, debe devolver false.
    bool conseguido = semaforo.tryAcquireFor(
        std::chrono::milliseconds(50)
    );

    // Comprobamos el resultado.
    if (conseguido) {

        // Si devuelve true, la prueba ha fallado.
        std::cout << "ERROR: se ha conseguido un permiso inexistente.\n";
        return 1;
    }

    // Si devuelve false, el semaforo funciona correctamente
    // en esta primera prueba.
    std::cout << "OK: prueba 1 superada.\n";

    return 0;
}
