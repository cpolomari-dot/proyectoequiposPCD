// main del caso. Vuestro trabajo: rellenar ejecutarSimulacion y las listas de claves y modos.
// Mirad ejemplo/main.cpp para ver cómo se usan las clases del proyecto base.
// Vuestras 3 clases van en esta misma carpeta (caso/), cada una en su .h y su .cpp,
// y sus .cpp se añaden a caso/CMakeLists.txt.

#include "Config.h"
#include "Estadisticas.h"
#include "Lanzador.h"
#include "Traza.h"
#include "Vigilante.h"

#include <chrono>
#include <string>
#include "Banco.h"
#include <thread>
#include <vector>



bool ejecutarSimulacion(const Config& config,
    Estadisticas& estadisticas, Traza& traza,
    const std::string& etiqueta) {

    // Leemos los parametros de configuracion del banco.
    const int clientes = config.entero("clientes", 50);
    const int cuentas = config.entero("cuentas", 10);
    const int operaciones = config.entero("operaciones", 20);
    const int distancia = config.entero("distancia", 1);
    const int umbral = config.entero("umbral", 100);

    // Comprobamos que la configuracion sea valida.
    if (clientes <= 0 || cuentas < 2 ||
        operaciones <= 0 || distancia < 1 ||
        distancia >= cuentas || umbral < 0) {

        estadisticas.violacion("Configuracion incorrecta");
        return false;
    }

    // La traza se utilizara cuando se complete la integracion.
    (void)traza;

    // Creamos el vigilante para detectar bloqueos.
    Vigilante vigilante(
        std::chrono::milliseconds(
            config.entero("vigilante", 2000)),
        estadisticas, etiqueta
    );

    // Iniciamos el vigilante.
    vigilante.iniciar();

    // Creamos el objeto principal del banco.
    Banco banco(cuentas, distancia, operaciones, umbral);

    // Vector donde guardamos los hilos clientes.
    std::vector<std::thread> hilosClientes;

    // Reservamos espacio para todos los clientes.
    hilosClientes.reserve(clientes);

    // Creamos un hilo por cada cliente.
    for (int i = 0; i < clientes; ++i) {

        // Cada hilo ejecuta las operaciones de su cliente.
        hilosClientes.emplace_back([&banco, i]() {
            banco.cliente(i);
            });
    }

    // Esperamos a que todos los clientes terminen.
    for (auto& hilo : hilosClientes) {
        hilo.join();
    }

    // Pendiente: integrar auditor, nomina y antifraude.
    // Pendiente: comprobar los invariantes finales.

    // Detenemos el vigilante al finalizar.
    vigilante.detener();

    // Devolvemos el resultado de las comprobaciones.
    return estadisticas.sinViolaciones();
}




int main(int argc, char* argv[]) {

    // Recogemos los argumentos introducidos por el usuario
    // desde la terminal al ejecutar nuestro programa.
    Config config(argc, argv);

    // Creamos el lanzador del caso 3: Banco.
    // Indicamos las seis claves permitidas por el profesor
    // para configurar el comportamiento de la simulacion.

    Lanzador lanzador(
        "banco",
        {
            "clientes",        // Numero de clientes.
            "cuentas",         // Numero de cuentas bancarias.
            "operaciones",     // Operaciones por cliente.
            "distancia",       // Distancia entre cuentas.
            "umbral",          // Importe sospechoso.
            "periodo_auditor"  // Tiempo entre auditorias.
        },
        {
            "carrera",         // Carrera de datos.
            "interbloqueo",    // Bloqueo entre hilos.
            "inanicion"        // Dificultad para acceder al recurso.
        }
    );

    // El lanzador comprueba los argumentos y ejecuta
    // nuestra funcion principal de simulacion.
    // Tambien permite realizar pruebas de estres.
    return lanzador.ejecutar(config, ejecutarSimulacion);
}
