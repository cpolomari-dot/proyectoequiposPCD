# Registro de uso de herramientas de IA (`IA.md`)

Registro de consultas realizadas con herramientas de IA conforme al apartado 1.3 del enunciado.

---

## Registro de Nata (P4 — Antifraude)

### Consulta 1 — Funcionamiento y estructura de la parte de Antifraude
- **Fecha:** 2026-11-23
- **Miembro:** Nata
- **Herramienta:** Antigravity (Google Deepmind)
- **Prompt:**
  > *"Oye me ha tocado la parte de antifraude en el proyecto del banco (caso 3). ¿Me puedes explicar qué tiene que hacer exactamente esta clase y cómo se comunica con los clientes y con el cierre al mandar el -1?"*
- **Respuesta obtenida (resumen):** Explicación de la arquitectura del antifraude como un productor-consumidor acotado: los clientes que hagan transferencias $\ge 100$ € meten la cuenta origen en la cola, el hilo antifraude las saca de una en una simulando el análisis (espera de 1 ms) y suma a las estadísticas. Para terminar, cuando acaban los clientes se encola el valor centinela `-1` y el hilo antifraude sale del bucle al leerlo.
- **Qué se incorporó:** El diseño de los métodos de la clase `Antifraude` (`encolar`, `desencolar` y `bucleHilo`) y la condición de salida limpia con `-1`.
- **Qué se descartó o corrigió y por qué:** Tenía dudas de si el antifraude tenía que acceder a los saldos o bloquear las cuentas bancarias; se descartó por completo porque el enunciado indica que no coge la cuenta, no mira el saldo y no bloquea ninguna operación.
- **Ficheros y commits:** `caso/Antifraude.h` y `caso/Antifraude.cpp`.

### Consulta 2 — Sincronización de la cola: semáforos y exclusión mutua
- **Fecha:** 2026-11-23
- **Miembro:** Nata
- **Herramienta:** Antigravity (Google Deepmind)
- **Prompt:**
  > *"Tengo que hacer la cola acotada de tamaño 8 usando el Semaforo propio para los huecos, un counting_semaphore para los elementos y un mutex para la cola. ¿En qué orden se hacen los acquire y release para que no se me quede colgado?"*
- **Respuesta obtenida (resumen):** Detalle del orden de sincronización: primero se adquiere el semáforo correspondiente (`_huecos.acquire()` al meter o `_elementos.acquire()` al sacar) para esperar a que haya espacio o dato, y justo después se protege el acceso a la `std::queue` con `std::lock_guard<std::mutex>`. Si se cogiera el mutex antes del semáforo, el hilo se dormiría reteniendo el cerrojo y ningún otro hilo podría entrar para desbloquearlo (interbloqueo).
- **Qué se incorporó:** Implementación de `encolar()` y `desencolar()` con el orden correcto de semáforos y cerrojos en `Antifraude.cpp`.
- **Qué se descartó o corrigió y por qué:** Al principio pensaba poner `vigilante.progreso()` dentro del hilo antifraude, pero tras revisar el enunciado del Caso 3 vimos que el progreso solo lo deben marcar las operaciones de los clientes. Si lo marcara el antifraude, podría ocultar un interbloqueo de clientes.
- **Ficheros y commits:** `caso/Antifraude.cpp` y tabla de predicados en `DISENO.md`.
