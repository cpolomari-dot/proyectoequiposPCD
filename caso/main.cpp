#include "Config.h"
#include "Estadisticas.h"
#include "Lanzador.h"
#include "Traza.h"
#include "Vigilante.h"
#include "Banco.h"
#include "Antifraude.h"
#include "Contexto.h"

#include <chrono>
#include <string>
#include <thread>
#include <vector>

bool ejecutarSimulacion(
    const Config& config,
    Estadisticas& estadisticas,
    Traza& traza,
    const std::string& etiqueta) {

    // Leemos los parametros de configuracion.
    const int clientes = config.entero("clientes", 50);
    const int cuentas = config.entero("cuentas", 10);
    const int operaciones = config.entero("operaciones", 20);
    const int distancia = config.entero("distancia", 1);
    const int umbral = config.entero("umbral", 100);
    const int periodoAuditor = config.entero("periodo_auditor", 20);

    // Validamos los parametros.
    if (clientes <= 0 || cuentas < 2 ||
        operaciones <= 0 || distancia < 1 ||
        distancia >= cuentas || umbral < 0 ||
        periodoAuditor <= 0) {

        estadisticas.violacion("Configuracion incorrecta");
        return false;
    }

    // Seleccionamos los modos de fallo.
    const bool carrera = config.fallo("carrera");
    const bool interbloqueo = config.fallo("interbloqueo");
    const bool inanicion = config.fallo("inanicion");

    // Creamos el vigilante.
    Vigilante vigilante(
        std::chrono::milliseconds(
            config.entero("vigilante", 2000)),
        estadisticas, etiqueta
    );

    vigilante.iniciar();

    // Creamos el banco.
    Banco banco(cuentas, distancia, operaciones,
        umbral, carrera, interbloqueo);

    // Configuramos el auditor.
    banco.configurarAuditor(
        periodoAuditor, inanicion, estadisticas);

    // Creamos el antifraude y el contexto.
    Antifraude antifraude;

    Contexto ctx{
        config, estadisticas, traza, vigilante
    };

    // Iniciamos el hilo antifraude.
    std::thread hiloAntifraude([&]() {
        antifraude.bucleHilo(ctx);
        });

    // Iniciamos el hilo auditor.
    std::thread hiloAuditor([&banco]() {
        banco.auditor();
        });

    // Iniciamos el hilo nomina.
    std::thread hiloNomina([&banco]() {
        banco.nomina();
        });

    // Creamos los hilos clientes.
    std::vector<std::thread> hilosClientes;
    hilosClientes.reserve(clientes);

    for (int i = 0; i < clientes; ++i) {
        hilosClientes.emplace_back([&banco, i]() {
            banco.cliente(i);
            });
    }

    // Esperamos a todos los clientes.
    for (auto& hilo : hilosClientes) {
        hilo.join();
    }

    // Cerramos el auditor y la nomina.
    banco.cerrarAuditorYNomina();
    hiloAuditor.join();
    hiloNomina.join();

    // Cerramos el antifraude mediante el centinela.
    antifraude.encolar(-1);
    hiloAntifraude.join();

    // Comprobamos los invariantes finales.
    banco.comprobarinvariantesFinales();

    // Detenemos el vigilante.
    vigilante.detener();

    // Devolvemos el resultado de la simulacion.
    return estadisticas.sinViolaciones();
}

int main(int argc, char* argv[]) {

    Config config(argc, argv);

    Lanzador lanzador(
        "banco",
        {
            "clientes",
            "cuentas",
            "operaciones",
            "distancia",
            "umbral",
            "periodo_auditor"
        },
        {
            "carrera",
            "interbloqueo",
            "inanicion"
        }
    );

    return lanzador.ejecutar(
        config, ejecutarSimulacion);
}