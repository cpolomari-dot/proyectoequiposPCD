#pragma once
#include <vector>
#include "Cuenta.h"
#include "Estadisticas.h"
#include <mutex>
#include <condition_variable>

class Banco {
public:
    Banco(int cuentas, int distancia, int operaciones,
        int umbral, bool carrera = false,
        bool interbloqueo = false);

    void cliente(int i); // el hilo del cliente i
    // Funciones desarrolladas por P3.
    void auditor();
    void nomina();
    void cerrarAuditorYNomina();
    void comprobarinvariantesFinales();

    // Configuracion del auditor y del modo inanicion.
    void configurarAuditor(int periodo, bool inanicion,
        Estadisticas& estadisticas);
    long total() const;       // suma de saldos (provisional, para probar)

private:
    std::vector<Cuenta> cuentas_;
    int distancia_;
    int operaciones_;
    int umbral_;
    // Indica si esta activado el modo de fallo carrera.
    bool carrera_;
    // Indica si esta activado el modo interbloqueo.
    bool interbloqueo_;
    // Configuracion del auditor.
    int periodo_auditor_ = 20;
    bool fallo_inanicion_ = false;
    Estadisticas* estadisticas_ = nullptr;

    // Variables para detener el auditor y la nomina.
    std::mutex mtx_estado_;
    std::condition_variable cv_cierre_;
    bool detener_hilos_ = false;

    // Registro de ingresos y retiradas.
    std::mutex mtx_totales_;
    long long ingresos_ = 0;
    long long retiradas_ = 0;

    // Funcion auxiliar desarrollada por P3.
    void comprobarInvariantesConCuentasBloqueadas();
};