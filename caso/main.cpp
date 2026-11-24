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

    // Recogemos los argumentos que introduce el usuario
    // al ejecutar el programa desde la terminal.
    Config config(argc, argv);

    // Creamos el lanzador de nuestro caso de estudio.
    // Nuestro proyecto corresponde al caso 3: Banco.
    //
    // Dejamos pendiente la lista de las seis claves
    // de configuracion hasta comprobar el enunciado.
    //
    // Indicamos los tres modos de fallo que tendremos
    // que implementar durante el desarrollo:
    //
    // carrera: acceso incorrecto a datos compartidos.
    // interbloqueo: hilos bloqueados entre ellos.
    // inanicion: un hilo no consigue acceder a un recurso.

    Lanzador lanzador(
        "banco",
        {},
        { "carrera", "interbloqueo", "inanicion" }
    );

    // El lanzador se encarga de ejecutar la simulacion
    // utilizando la configuracion indicada por el usuario.
    return lanzador.ejecutar(config, ejecutarSimulacion);
}
