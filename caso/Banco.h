#include "Cuenta.h"
#include "Estadisticas.h"

class Banco {
public:
    Banco(int cuentas, int distancia, int operaciones, int umbral);

    void cliente(int i);      // el hilo del cliente i
    void auditor();
    long total() const;       // suma de saldos (provisional, para probar)

    //Cierre y comprobación de invariantes

	void cerrarAuditorYNomina(); // Indica a los hilos del auditor y de la nómina que deben detenerse y notifica a los hilos que están esperando en las condiciones de cierre
    void comprobarinvariantesFinales();
    long total() const;

    //Registro de ingresos y retiradas para el invariante suma

    void registrarIngreso(long cantidad);
	void registrarRetirada(long cantidad);


private:

	void comprobarInvariantesConCuentasBloqueadas(); // Comprobar las invariantes con todas las cuentas bloqueadas y registrar las violaciones en las estadísticas

    std::vector<Cuenta> cuentas_;
    int distancia_;
    int operaciones_;
    int umbral_;

    // Parámetros y referencias necesarias para auditor, nómina e invariantes

    int periodo_auditor_ = 20;
    bool fallo_inanicion_ = false;
    Estadisticas* estadisticas_ = nullptr; // O referencia inicializada en constructor

    // Sincronización para el cierre limpio de auditor y nómina

    std::mutex mtx_estado_;
    std::condition_variable cv_cierre_;
    bool detener_hilos_ = false;

    // Totales para comprobar: suma == 200*N + ingresos - retiradas

    std::mutex mtx_totales_;
    long long ingresos_ = 0;
    long long retiradas_ = 0;
};
};