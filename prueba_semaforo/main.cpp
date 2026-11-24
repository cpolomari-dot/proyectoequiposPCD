
#include "Semaforo.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>

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



    // ==========================================
    // PRUEBA 2: Comprobar el funcionamiento de release()
    // ==========================================

    // En la primera prueba teniamos 0 permisos.
    // Ahora vamos a devolver un permiso al semaforo.
    semaforo.release();

    // Intentamos conseguir el permiso que acabamos de devolver.
    // Como ahora existe un permiso, deberia conseguirse.
    bool conseguido2 = semaforo.tryAcquireFor(
        std::chrono::milliseconds(50)
    );

    // Comprobamos si hemos conseguido el permiso.
    if (!conseguido2) {

        // Si no conseguimos el permiso, algo ha fallado.
        std::cout << "ERROR: release no ha devuelto el permiso.\n";
        return 1;
    }

    // Si llegamos aqui, significa que release funciona.
    std::cout << "OK: prueba 2 superada.\n";



    // ==============================================
    // PRUEBA 3: Bloquear y desbloquear un hilo
    // ==============================================

    // Creamos un nuevo semaforo sin permisos.
    // Como empieza en 0, cualquier hilo que intente
    // adquirir un permiso tendra que esperar.
    Semaforo semaforo3(0);

    // Esta variable nos permite comprobar si el hilo
    // ha conseguido pasar por el semaforo.
    // Usamos atomic para evitar problemas entre hilos.
    std::atomic<bool> haEntrado(false);

    // Creamos un hilo que intentara conseguir un permiso.
    std::thread hilo([&]() {

        // El hilo se bloquea aqui porque no hay permisos.
        semaforo3.acquire();

        // Esta linea solo se ejecuta cuando otro hilo
        // devuelve un permiso mediante release().
        haEntrado.store(true);
        });

    // Dejamos pasar 100 milisegundos para comprobar
    // que el hilo sigue esperando.
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)
    );

    // Si el hilo ha entrado sin que nadie devuelva
    // un permiso, significa que el semaforo falla.
    bool entroAntes = haEntrado.load();

    // Ahora devolvemos un permiso.
    // Esto permite que el hilo bloqueado continue.
    semaforo3.release();

    // Esperamos a que termine el hilo antes de comprobar.
    // Esto evita dejar hilos ejecutandose al salir.
    hilo.join();

    // Comprobamos que no haya entrado antes de release()
    // y que si haya entrado despues.
    if (entroAntes || !haEntrado.load()) {
        std::cout << "ERROR: fallo al bloquear o desbloquear el hilo.\n";
        return 1;
    }

    // Si llegamos aqui, la tercera prueba ha funcionado.
    std::cout << "OK: prueba 3 superada.\n";


    return 0;
}