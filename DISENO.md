# Diseño de concurrencia — Caso 3 (Banco)

## 1. Monitor `Cuenta` (Parte P2 — Pouria / P1)

La clase `Cuenta` está implementada como un monitor que encapsula el saldo bancario, un mutex para garantizar la exclusión mutua y una variable de condición para la sincronización condicional (Tema 5 y Tema 7).

### Variables privadas
- `saldo_`: entero largo con el saldo actual de la cuenta (inicia en 200 €).
- `m_`: `std::mutex` que protege el acceso a `saldo_`.
- `cv_`: `std::condition_variable` utilizada para bloquear hilos clientes que intentan retirar sin suficiente saldo y despertarlos cuando entra dinero.

### Tabla de predicados de `Cuenta`

| Operación | Espera mientras… | Quién avisa |
|---|---|---|
| `retirar(importe, timeout)` | `saldo_ < importe` (con límite de 20 ms) | `ingresar()` y `transferir()` (en cuenta destino) mediante `cv_.notify_all()` |
| `ingresar(importe)` | No espera por condición (adquiere `std::unique_lock<std::mutex>`) | Avisa a los hilos en espera mediante `cv_.notify_all()` |
| `transferir(origen, destino, importe)` | No espera por condición (adquiere ambos cerrojos con `std::scoped_lock`) | Avisa a los hilos en espera de la cuenta destino con `destino.cv_.notify_all()` |
| `saldo()` | No espera por condición (adquiere cerrojo de lectura) | No aplica |

### Decisiones de sincronización
1. **RAII estricto:** 
   - En `retirar()` e `ingresar()` se emplea `std::unique_lock<std::mutex>` para permitir que la variable de condición libere temporalmente el cerrojo durante la espera y lo vuelva a adquirir al despertar.
   - En `transferir()` se utiliza `std::scoped_lock` bloqueando simultáneamente `origen.m_` y `destino.m_`, evitando problemas de inversión de cerrojos (interbloqueo).
2. **Espera con tiempo límite:**
   - La retirada utiliza `cv_.wait_for(lk, timeout, [&] { return saldo_ >= importe; })`. Si tras el tiempo máximo (20 ms) no hay saldo suficiente, la operación se cancela sin modificar el saldo, registrando la duración de espera en `Estadisticas::registrarEspera("cliente", duracion)`.
3. **Avisos oportunos:**
   - Cada vez que una cuenta recibe dinero (mediante un ingreso directo o como destino de una transferencia), se emite `cv_.notify_all()` para evaluar si algún hilo en espera puede completar su retirada.
4. **Simulación de duración:**
   - Todas las operaciones del cliente mantienen sus cuentas cogidas durante 1 ms mediante `std::this_thread::sleep_for(std::chrono::milliseconds(1))` conforme a los requisitos del enunciado.

---

## 2. Competición (`competicion/Producto.cpp`) (Parte P2 — Pouria)

Para el cálculo del producto acumulado módulo `1 000 000 007` de los `2 000 000` valores generados por `valor(i)`:
- Se divide el rango `[0, N_VALORES)` de manera uniforme y balanceada entre los `hilos` asignados.
- Cada hilo calcula independientemente el subproducto de su intervalo con acumulador local, eliminando contención de cerrojos y falsos compartidos (*false sharing*).
- Al finalizar (`join`), el hilo principal multiplica los resultados parciales de cada hilo módulo `1 000 000 007`, asegurando exactitud matemática y máxima aceleración multinúcleo.

---

## 3. Otros monitores y componentes del equipo

- **`Antifraude` (P4 — Nata):** Cola acotada con patrón productor-consumidor usando `Semaforo` y `std::counting_semaphore`.
- **`Semaforo` y modos de fallo (P5 — Manuel):** Monitor de semáforo con `acquire`, `release` y `tryAcquireFor`.
- **`Auditor` y `Nomina` (P3 — Quique):** Pasadas periódicas sobre todas las cuentas comprobando invariantes.
