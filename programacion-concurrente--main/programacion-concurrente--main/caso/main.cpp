
#include "Config.h"
#include "Contexto.h"
#include "Estadisticas.h"
#include "Lanzador.h"
#include "Traza.h"
#include "Vigilante.h"
#include "Banco.h"

#include <chrono>
#include <string>

bool ejecutarSimulacion(
    const Config& config,
    Estadisticas& estadisticas,
    Traza& traza,
    const std::string& etiqueta
) {
    Vigilante vigilante(
        std::chrono::milliseconds(
            config.entero("vigilante", 2000)
        ),
        estadisticas,
        etiqueta
    );

    Contexto ctx{
        config,
        estadisticas,
        traza,
        vigilante
    };

    Banco banco(ctx);

    vigilante.iniciar();

    bool resultado = banco.ejecutar();

    vigilante.detener();

    return resultado && estadisticas.sinViolaciones();
}

int main(int argc, char* argv[]) {
    Config config(argc, argv);

    Lanzador lanzador(
        "banco",
        {"clientes", "operaciones", "cantidad"},
        {"carrera"}
    );

    return lanzador.ejecutar(config, ejecutarSimulacion);
}
