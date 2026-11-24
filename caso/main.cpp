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

    // ==============================================
    // CONFIGURACION DE LA SIMULACION DEL BANCO
    // ==============================================

    // Leemos el numero de clientes que operaran.
    // Si el usuario no indica otro valor, seran 50.
    const int clientes = config.entero("clientes", 50);

    // Numero de cuentas bancarias disponibles.
    const int cuentas = config.entero("cuentas", 10);

    // Numero de operaciones que realizara cada cliente.
    const int operaciones = config.entero("operaciones", 20);

    // Distancia entre las dos cuentas de cada cliente.
    const int distancia = config.entero("distancia", 1);

    // Importe minimo para considerar una
    // transferencia como sospechosa.
    const int umbral = config.entero("umbral", 100);

    // Tiempo en milisegundos entre las comprobaciones
    // realizadas por el auditor del banco.
    const int periodoAuditor =
        config.entero("periodo_auditor", 20);

    // Validamos los parametros antes de crear los hilos.
    // Asi evitamos configuraciones incorrectas.
    if (clientes <= 0 ||
        cuentas < 2 ||
        operaciones <= 0 ||
        distancia < 1 ||
        distancia >= cuentas ||
        umbral < 0 ||
        periodoAuditor <= 0) {

        estadisticas.violacion("Configuracion del banco incorrecta");
        return false;
    }

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
