#include "Banco.h"
#include <chrono>
#include <thread>
#include <string>
#include <utility>

Banco::Banco(int cuentas, int distancia, int operaciones,
    int umbral, bool carrera, bool interbloqueo)
    : cuentas_(cuentas),
    distancia_(distancia),
    operaciones_(operaciones),
    umbral_(umbral),
    carrera_(carrera),
    interbloqueo_(interbloqueo) {
}

void Banco::cliente(int i) {
    const int n = static_cast<int>(cuentas_.size());
    const int a = i % n;
    const int b = (i + distancia_) % n;

    for (int j = 0; j < operaciones_; ++j) {
        const int origen  = (j % 2 == 0) ? a : b;
        const int destino = (j % 2 == 0) ? b : a;

        if (j % 5 <= 2) {                                   // tipos 0, 1, 2: transferencia
            const long importe = 10 + (13L * i + 7L * j) % 100;
            // Realizamos la transferencia utilizando el modo
// de funcionamiento seleccionado en la configuracion.
            // Ejecutamos la transferencia con el modo
// de fallo seleccionado por el usuario.
            const bool hecha = Cuenta::transferir(
                cuentas_[origen],
                cuentas_[destino],
                importe,
                carrera_,
                interbloqueo_
            );
            if (hecha && importe >= umbral_) {
                // TODO (P4): poner la cuenta origen en la cola antifraude
            }
        }
        // TODO (P2): j % 5 == 3 es un ingreso de 30, y j % 5 == 4 una retirada de 60
        // TODO: Vigilante::progreso() y la métrica "operaciones hechas"
    }
}

long Banco::total() const {
    long suma = 0;
    for (const Cuenta& c : cuentas_) suma += c.saldo();
    return suma;
}

// Configuramos el auditor antes de iniciar los hilos.
void Banco::configurarAuditor(
    int periodo, bool inanicion,
    Estadisticas& estadisticas) {

    // Guardamos el periodo entre auditorias.
    periodo_auditor_ = periodo;

    // Activamos o desactivamos el modo de fallo.
    fallo_inanicion_ = inanicion;

    // Guardamos la direccion de las estadisticas.
    estadisticas_ = &estadisticas;
}

// Comprobamos los invariantes al finalizar la simulacion.
void Banco::comprobarinvariantesFinales() {

    // Bloqueamos todas las cuentas.
    std::vector<std::unique_lock<std::mutex>> locks;

    locks.reserve(cuentas_.size());

    for (auto& cuenta : cuentas_) {
        locks.emplace_back(cuenta.mutex());
    }

    // Ejecutamos la comprobacion desarrollada por P3.
    comprobarInvariantesConCuentasBloqueadas();
}
//Parte 3 - Quique

void Banco::auditor() {
    while (true) {
        {
			std::unique_lock<std::mutex> lock(mtx_estado_); // Bloquea el mutex para esperar a que se detengan los hilos
            cv_cierre_.wait_for(
                lock,

                std::chrono::milliseconds(periodo_auditor_),
                [this]() { return detener_hilos_; }
            );

			if (detener_hilos_) { // Si se ha solicitado detener los hilos, salir del bucle
                break;
            }
        }

        auto inicio = std::chrono::steady_clock::now();

        if (fallo_inanicion_) { // Si se ha activado el fallo de inanici├│n, intentar bloquear todas las cuentas sin orden
            bool hecha = false;
            while (!hecha) {
                {
                    std::lock_guard<std::mutex> lock(mtx_estado_);
                    if (detener_hilos_) {
                        return;
                    }
                }

                std::vector<std::unique_lock<std::mutex>> locks;
                locks.reserve(cuentas_.size());
                bool ok = true;

                for (size_t idx = 0; idx < cuentas_.size(); ++idx) { // Intentar bloquear cada cuenta sin orden

                    std::unique_lock<std::mutex> lk(cuentas_[idx].mutex(), std::try_to_lock);
                    if (!lk.owns_lock()) {

                        ok = false;
                        break;
                    }
                    locks.push_back(std::move(lk));
                }

                if (!ok) { //Si no se ha podido bloquear todas las cuentas, liberar los locks y esperar antes de reintentar

                    locks.clear();
                    estadisticas_->sumar("pasadas fallidas", 1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }

				comprobarInvariantesConCuentasBloqueadas(); // Si se han bloqueado todas las cuentas, comprobar las invariantes y registrar la m├®trica de auditor├¡a
                estadisticas_->sumar("pasadas del auditor", 1);
                hecha = true;
            }
        }

        else {

			std::vector<std::unique_lock<std::mutex>> locks; // Bloquear todas las cuentas en orden para evitar inanici├│n
            locks.reserve(cuentas_.size());
            for (size_t idx = 0; idx < cuentas_.size(); ++idx) {
                locks.emplace_back(cuentas_[idx].mutex());
            }

			comprobarInvariantesConCuentasBloqueadas(); // Comprobar las invariantes con todas las cuentas bloqueadas y registrar la m├®trica de auditor├¡a
            estadisticas_->sumar("pasadas del auditor", 1);
        }

		auto fin = std::chrono::steady_clock::now(); // Registrar el tiempo que ha tardado la auditor├¡a y a├▒adirlo a las estad├¡sticas
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
        estadisticas_->registrarEspera("auditor", ms);
    }
}

void Banco::nomina() {

	while (true) { // Ingresa 10 euros en cada cuenta, avisando a las retiradas que est├ín esperando
        {
            std::unique_lock<std::mutex> lock(mtx_estado_);
            cv_cierre_.wait_for(
                lock,
                std::chrono::milliseconds(30),
                [this]() { return detener_hilos_; }
            );

            if (detener_hilos_) {
                break;
            }
        }

		for (size_t idx = 0; idx < cuentas_.size(); ++idx) { // Para cada cuenta, intentar ingresar 10 euros y avisar a las retiradas que est├ín esperando
            {
                std::lock_guard<std::mutex> lock(mtx_estado_);
                if (detener_hilos_) {
                    return;
                }
            
            cuentas_[idx].ingresar(10); // Ingresa 10 euros en la cuenta idx (dura 1 ms y avisa a retiradas esperando)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
}
	void Banco::cerrarAuditorYNomina() { // Indica a los hilos del auditor y de la n├│mina que deben detenerse y notifica a los hilos que est├ín esperando en las condiciones de cierre
    {
        std::lock_guard<std::mutex> lock(mtx_estado_);
        detener_hilos_ = true;
    }
    cv_cierre_.notify_all();
}
    // Funci├│n auxiliar de Banco

	void Banco::comprobarInvariantesConCuentasBloqueadas() { // Comprobar las invariantes con todas las cuentas bloqueadas y registrar las violaciones en las estad├¡sticas

        const long long n = static_cast<long long>(cuentas_.size());
        long long sumaSaldos = 0;

        for (size_t i = 0; i < cuentas_.size(); ++i) { // Leer el saldo sin volver a bloquear el mutex ya adquirido por el auditor

            const long long saldo = cuentas_[i].saldoSinBloquear();

			if (saldo < 0) { // Si el saldo es negativo, registrar la violaci├│n en las estad├¡sticas

                estadisticas_->violacion("Saldo negativo en la cuenta " + std::to_string(i));
            }
            sumaSaldos += saldo;
        }

        long long totalIngresos = 0;
        long long totalRetiradas = 0;

        {
			std::lock_guard<std::mutex> lock(mtx_totales_); // Bloquear el mutex para leer los totales de ingresos y retiradas sin interferencias de otros hilos
            totalIngresos = ingresos_;
            totalRetiradas = retiradas_;
        }

        const long long saldoEsperado = 200LL * n + totalIngresos - totalRetiradas;

		if (sumaSaldos != saldoEsperado) { // Si la suma de saldos no coincide con el saldo esperado, registrar la violaci├│n en las estad├¡sticas

            estadisticas_->violacion(
                "Invariante de suma roto, el actual es: " + std::to_string(sumaSaldos) + "y el esperado es: " + std::to_string(saldoEsperado)
            );
        }
    }

