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

bool ejecutarSimulacion(const Config& config, Estadisticas& estadisticas, Traza& traza, const std::string& etiqueta) {
    (void)traza;
    Vigilante vigilante(std::chrono::milliseconds(config.entero("vigilante", 2000)), estadisticas, etiqueta);
    vigilante.iniciar();

    // TODO: crear los objetos, lanzar los hilos, cierre ordenado, invariantes finales.

    vigilante.detener();
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
