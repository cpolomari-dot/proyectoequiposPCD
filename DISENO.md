## P5 — Semáforo propio

### Descripción

El semáforo está implementado como un monitor que permite controlar cuántos hilos pueden acceder simultáneamente a un recurso compartido.

### Variables privadas

- `_valor`: número de permisos disponibles.
- `_m`: mutex que protege el contador de permisos.
- `_cv`: variable de condición que permite bloquear y despertar hilos.

### Tabla de predicados

| Operación | Espera mientras… | Quién avisa |
|---|---|---|
| `acquire()` | `_valor <= 0` | `release()` mediante `notify_one()` |
| `release()` | No espera por permisos. Adquiere el mutex. | Avisa a un hilo mediante `notify_one()` |
| `tryAcquireFor()` | `_valor <= 0` y no ha vencido el plazo | `release()` mediante `notify_one()` |

### Sincronización

Se utiliza `std::unique_lock` en `acquire()` y `tryAcquireFor()` porque la variable de condición necesita liberar temporalmente el mutex durante la espera.

En `release()` se utiliza `std::lock_guard` para proteger el incremento del contador.

Todos los bloqueos se gestionan mediante RAII, evitando llamadas manuales a `lock()` y `unlock()`.

### Invariante

El número de permisos disponibles nunca debe ser negativo, partiendo de un valor inicial válido.

### Pruebas realizadas

- Semáforo sin permisos.
- Devolución y adquisición de permisos.
- Bloqueo y desbloqueo entre hilos.
- Concurrencia con 12 hilos y 20 repeticiones por hilo.
- Límite de 3 hilos simultáneos.
- Comparación con `std::counting_semaphore`.
- Tiempo de espera máximo de 50 ms sin permisos.

### Aplicación en el banco

El semáforo propio se utiliza para controlar los huecos disponibles en la cola antifraude, cuya capacidad es de 8 elementos.