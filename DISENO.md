# Diseño de concurrencia — Caso 3 (Banco)

## 1. Cola `Antifraude` (Parte P4 — Nata)

La clase `Antifraude` implementa una cola acotada de capacidad fija `CAPACIDAD = 8` siguiendo el patrón productor-consumidor con semáforos (Tema 6, Regla 3 del enunciado):
- `_huecos`: `Semaforo` propio (monitor), inicializado a `8`.
- `_elementos`: `std::counting_semaphore<8>`, inicializado a `0`.
- `_mutexCola`: `std::mutex` para exclusión mutua sobre `std::queue<int> _cola` (siempre mediante `std::lock_guard<std::mutex>`).
- Cierre ordenado: se encola el valor centinela `-1`; al desencolarlo, el hilo antifraude sale del bucle y termina.

### Tabla de predicados y sincronización (`Antifraude`)

| Operación | Espera mientras… | Quién avisa |
|---|---|---|
| `Antifraude::encolar(cuenta)` | No hay huecos libres (`_huecos == 0`, cola llena con 8 elementos) | `Antifraude::desencolar()` al llamar a `_huecos.release()` tras extraer un elemento |
| `Antifraude::desencolar()` | No hay elementos disponibles (`_elementos == 0`, cola vacía) | `Antifraude::encolar(cuenta)` al llamar a `_elementos.release()` tras insertar una cuenta (o `-1` en el cierre) |

> **Orden de adquisición:** tanto `encolar` como `desencolar` adquieren primero su semáforo (`_huecos` / `_elementos`) y **después** bloquean `_mutexCola`. Nunca se duerme manteniendo `_mutexCola` cogido, evitando así el interbloqueo entre productor y consumidor.

---

## 2. Otros monitores del Caso 3 (pendiente de completar por el resto del equipo)

- **`Semaforo` (P5):** pendiente de completar por P5 (`acquire()`, `tryAcquireFor()`, `release()`).
- **`Cuenta` (P1 / P2):** pendiente de completar por P1 y P2 (`retirar()` esperando saldo con `wait_for(20 ms)`, avisado por `ingresar()` y `transferir()`).
