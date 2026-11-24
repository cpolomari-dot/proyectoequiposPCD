# Registro de uso de herramientas de IA (`IA.md`)

Registro de consultas realizadas con herramientas de IA conforme al apartado 1.3 del enunciado.

---

## Registro de Pouria (P2 — Retiradas y Competición)

### Consulta 1 — Sincronización de retiradas con espera temporal en variable de condición
- **Fecha:** 2026-11-24
- **Miembro:** Pouria (P2)
- **Herramienta:** Antigravity (Google Deepmind)
- **Prompt:**
  > *"En el caso 3 (Banco), tengo asignada la parte P2 (retiradas con espera de saldo de máx. 20 ms, ingresos que avisan y avisos en transferencias). ¿Cómo se debe estructurar el método retirar() en Cuenta utilizando std::condition_variable y std::unique_lock para respetar el límite de 20 ms y evitar despertares espurios?"*
- **Respuesta obtenida (resumen):** Uso de `cv_.wait_for` pasando como predicado lambda `[this, importe]() { return saldo_ >= importe; }`. Esto garantiza que el hilo sólo despierte si se satisface la condición o si vence el plazo de 20 ms. Durante la espera se mide el tiempo transcurrido con `std::chrono::steady_clock` y se anota con `estadisticas.registrarEspera("cliente", duracion)`.
- **Qué se incorporó:** La implementación completa de `retirar()` en `caso/Cuenta.h` y `caso/Cuenta.cpp`, así como la gestión del timeout y la deducción segura del saldo.
- **Qué se descartó o corrigió:** Se verificó que no se debe utilizar espera activa ni bucles con sleep manuales; todo se realiza con la variable de condición y RAII.
- **Ficheros y commits:** `caso/Cuenta.h`, `caso/Cuenta.cpp`, `caso/Banco.cpp`.

### Consulta 2 — Mecanismo de notificación desde ingresos y transferencias
- **Fecha:** 2026-11-24
- **Miembro:** Pouria (P2)
- **Herramienta:** Antigravity (Google Deepmind)
- **Prompt:**
  > *"¿Cómo deben coordinarse ingresar() y transferir() con retirar() para que los clientes esperando saldo sean avisados inmediatamente cuando llegue dinero?"*
- **Respuesta obtenida (resumen):** Tanto `ingresar()` como `transferir()` deben llamar a `cv_.notify_all()` sobre la cuenta receptora del dinero justo después de incrementar su saldo y antes de finalizar la sección crítica. `notify_all()` asegura que si varios hilos esperan saldo en esa misma cuenta, todos evalúen sus predicados.
- **Qué se incorporó:** Notificaciones `cv_.notify_all()` en `Cuenta::ingresar` y en la cuenta destino de `Cuenta::transferir`.
- **Qué se descartó o corrigió:** Se consideró `notify_one()`, pero se descartó en favor de `notify_all()` porque una transferencia grande podría permitir que varias retiradas más pequeñas se desbloqueen simultáneamente.
- **Ficheros y commits:** `caso/Cuenta.cpp`, `DISENO.md`.

### Consulta 3 — Paralelización eficiente de la función de competición
- **Fecha:** 2026-11-24
- **Miembro:** Pouria (P2)
- **Herramienta:** Antigravity (Google Deepmind)
- **Prompt:**
  > *"Tengo que implementar producto(int hilos) en competicion/Producto.cpp para calcular el producto módulo 1 000 000 007 de 2 000 000 valores llamando exactamente una vez a valor(i). ¿Cuál es la forma más rápida y conforme a las reglas usando std::thread?"*
- **Respuesta obtenida (resumen):** Dividir el espacio de índices `[0, N_VALORES)` en `hilos` bloques contiguos. Cada hilo almacena su producto acumulado en un elemento de un vector de resultados sin necesidad de cerrojos durante el cómputo. Al finalizar los hilos con `join()`, el hilo principal combina los subproductos de cada hilo.
- **Qué se incorporó:** Implementación de `competicion/Producto.cpp` verificada con `./competicion --comprobar`.
- **Ficheros y commits:** `competicion/Producto.cpp`, `DISENO.md`.
