
#include "Semaforo.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <algorithm>

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




    // ==============================================
    // PRUEBA 4: Concurrencia con 12 hilos
    // ==============================================

    // Creamos un semaforo con 3 permisos.
    // Como maximo pueden entrar 3 hilos a la vez.
    Semaforo semaforo4(3);

    // Vector donde guardamos los hilos creados.
    std::vector<std::thread> hilos;

    // Contador de hilos que estan dentro del semaforo.
    std::atomic<int> dentro(0);

    // Maximo numero de hilos simultaneos registrado.
    std::atomic<int> maximo(0);

    // Contador de operaciones completadas.
    std::atomic<int> completadas(0);

    // Indica si se ha superado el limite de 3 hilos.
    std::atomic<bool> error4(false);

    // Creamos los 12 hilos que exige el profesor.
    for (int i = 0; i < 12; ++i) {

        hilos.emplace_back([&]() {

            // Cada hilo repite el proceso 20 veces.
            for (int j = 0; j < 20; ++j) {

                // Esperamos hasta conseguir un permiso.
                semaforo4.acquire();

                // Aumentamos el numero de hilos dentro.
                int actual = ++dentro;

                // Consultamos el maximo registrado.
                int anterior = maximo.load();

                // Actualizamos el maximo de forma atomica.
                // Si otro hilo lo cambia, volvemos a intentar.
                while (actual > anterior &&
                    !maximo.compare_exchange_weak(
                        anterior, actual)) {
                }

                // Comprobamos el limite del semaforo.
                if (actual > 3) {
                    error4.store(true);
                }

                // Simulamos el trabajo durante los
                // 300 microsegundos que pide el profesor.
                std::this_thread::sleep_for(
                    std::chrono::microseconds(300)
                );

                // El hilo termina su trabajo y sale.
                --dentro;

                // Devolvemos el permiso al semaforo.
                semaforo4.release();

                // Registramos una operacion completada.
                ++completadas;
            }
            });
    }

    // Esperamos a que los 12 hilos terminen.
    for (auto& hilo4 : hilos) {
        hilo4.join();
    }

    // Comprobamos todos los resultados:
    // - No se ha superado el limite de 3 hilos.
    // - No queda ningun hilo dentro.
    // - Se han completado las 240 operaciones.
    if (error4.load() ||
        dentro.load() != 0 ||
        maximo.load() > 3 ||
        completadas.load() != 240) {

        std::cout << "ERROR: prueba 4 fallida.\n";
        return 1;
    }

    // Mostramos los resultados de la prueba.
    std::cout << "OK: prueba 4 superada.\n";

    std::cout << "Maximo de hilos simultaneos: "
        << maximo.load() << "\n";

    std::cout << "Operaciones completadas: "
        << completadas.load() << "\n";


    return 0;
}