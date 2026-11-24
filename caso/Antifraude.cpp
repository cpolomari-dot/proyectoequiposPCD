#include "Antifraude.h"

#include "Estadisticas.h"
#include "Traza.h"

#include <chrono>
#include <string>
#include <thread>

Antifraude::Antifraude()
    : _huecos(CAPACIDAD), _elementos(0) {}

void Antifraude::encolar(int cuenta) {
    // Espera a que haya un hueco libre en la cola (bloqueo si hay 8 elementos)
    _huecos.acquire();

    {
        std::lock_guard<std::mutex> lock(_mutexCola);
        _cola.push(cuenta);
    }

    // Notifica que hay un nuevo elemento disponible para analizar
    _elementos.release();
}

int Antifraude::desencolar() {
    // Espera a que haya al menos un elemento disponible
    _elementos.acquire();

    int cuenta = -1;
    // Sección crítica: extracción segura del frente de la cola
    {
        std::lock_guard<std::mutex> lock(_mutexCola);
        cuenta = _cola.front();
        _cola.pop();
    }

    // Notifica que se ha liberado un hueco en la cola
    _huecos.release();

    return cuenta;
}

void Antifraude::bucleHilo(const Contexto& ctx) {
    while (true) {
        int cuenta = desencolar();
        if (cuenta == -1) {
            // Valor centinela que indica el fin de la simulación
            break;
        }

        // Simulación del análisis: cada análisis antifraude dura 1 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        ctx.estadisticas.sumar("sospechosas analizadas", 1);
        ctx.traza.evento("antifraude", "analizada cuenta " + std::to_string(cuenta));
    }

    ctx.traza.evento("antifraude", "termina");
}
