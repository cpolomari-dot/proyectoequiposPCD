# Proyecto base — Trabajo en equipo Temas 3 a 8

Punto de partida obligatorio para los cinco casos. Cada carpeta genera **una** biblioteca o **un** ejecutable y tiene su propio `CMakeLists.txt`. El `CMakeLists.txt` de la raíz solo las reúne con `add_subdirectory`.

| Carpeta | Genera | Qué contiene | ¿Se modifica? |
|---|---|---|---|
| `base/` | biblioteca `base` | Clases auxiliares: `Config`, `Estadisticas`, `Traza`, `Vigilante`, `Lanzador` y la estructura `Contexto` | No |
| `semaforo/` | biblioteca `semaforo` | Vuestra clase `Semaforo` (la interfaz está fijada, pero la implementación es vuestra) | Sí: `Semaforo.cpp` |
| `prueba_semaforo/` | ejecutable `prueba_semaforo` | La prueba de `Semaforo` (apartado 1.4) | Sí: `main.cpp` |
| `caso/` | ejecutable de vuestro caso | `main.cpp` y vuestras 3 clases | Sí: todo, incluido su `CMakeLists.txt` |
| `competicion/` | ejecutable `competicion` | Programa de la competición (apartado 6) | Solo `Producto.cpp` |
| `ejemplo/` | ejecutable `ejemplo` | Ejemplo pequeño que usa las clases auxiliares, con los modos `carrera` e `inanicion` | No, es para mirar |

## Primeros pasos

1. En `caso/CMakeLists.txt`, poned el nombre del ejecutable de vuestro caso y, a medida que las creéis, añadid el `.h` y el `.cpp` de cada una de vuestras clases. Los `.h` también se añaden: si no, el IDE no los muestra en el proyecto.
2. Compilad y ejecutad el ejemplo: `./ejemplo`, `./ejemplo --traza`, `./ejemplo --fallo carrera --estres 10` y `./ejemplo --fallo inanicion` (mirad la espera máxima del hilo 0).
3. Copiad la estructura de `ejemplo/main.cpp` en `caso/main.cpp`.

Cada ejecutable tiene un solo `main`. No añadáis a `caso/CMakeLists.txt` ficheros de otras carpetas: daría un error de enlazado por `main` repetido.

## Convención de nombres

Los miembros privados de las clases llevan un guion bajo delante: `_saldo`, `_m`, `_cv`.

## Las clases auxiliares

**`Config`**: lee la línea de órdenes `--clave valor`.
- `config.entero("aviones", 30)`: valor de `--aviones`, o 30 si no se ha pasado.
- `config.fallo("carrera")`: `true` si se ha pasado `--fallo carrera`.
- `config.bandera("traza")`: `true` si se ha pasado `--traza`.

**`Estadisticas`**: todo es seguro entre hilos.
- `registrarEspera("avión (puerta)", duracion)`: anota una espera. La duración es la resta de dos `std::chrono::steady_clock::now()`.
- `sumar("operaciones", 1)`: suma a una métrica.
- `violacion("dos aviones en la pista 1")`: anota un invariante roto.
- `sinViolaciones()`: `true` si no se ha roto ningún invariante.

**`Traza`**: `traza.evento("avión 3", "aterriza en pista 1")` escribe una línea solo si se ha pasado `--traza`.

**`Vigilante`**: se crea dentro de la simulación (ver el ejemplo), se arranca con `iniciar()` y se para con `detener()` al final. Cada hilo llama a `vigilante.progreso()` al terminar una unidad de trabajo. Si pasan 2 s (o `--vigilante ms`) sin progreso, imprime las estadísticas y termina el programa con código 3.

**`Contexto`**: estructura que agrupa `config`, `estadisticas`, `traza` y `vigilante`. Pasad un `Contexto` al constructor de vuestras clases para no tener que pasar los cuatro objetos por separado.

**`Lanzador`**: se usa en `main`, con el nombre del caso, la lista de vuestras claves y la lista de modos de fallo. Se encarga de:
- comprobar las opciones (una clave o un modo desconocido termina con código 2).
- ejecutar `ejecutarSimulacion` una vez, o N veces con `--estres N`.
- imprimir las estadísticas.
- devolver el código de salida: 0 si todo está bien y 1 si se ha roto algún invariante.

## Competición

- `./competicion --hilos 4`: una medición con 4 hilos.
- `./competicion --hilos 4 --veces 5`: 5 mediciones y su mediana.
- `./competicion --comprobar`: calcula también el resultado correcto y dice si coincide (tarda más).

Para medir, compilad siempre en modo Release. Es el modo por defecto de este proyecto. En Visual Studio, elegid la configuración Release.


## Caso 3 — Banco: Semáforo (P5)

### Implementación del semáforo

El semáforo propio está implementado mediante un monitor que utiliza un mutex, una variable de condición y un contador de permisos.

Dispone de tres operaciones:

- `acquire()`: adquiere un permiso o bloquea el hilo hasta que haya uno disponible.
- `release()`: devuelve un permiso y notifica a un hilo que esté esperando.
- `tryAcquireFor()`: intenta adquirir un permiso durante un tiempo limitado.

La protección del estado compartido se realiza mediante RAII.

### Pruebas del semáforo

El ejecutable `prueba_semaforo` comprueba:

- Adquisición con cero permisos.
- Devolución de permisos.
- Bloqueo y desbloqueo de hilos.
- Concurrencia con 12 hilos.
- 20 repeticiones por hilo.
- Trabajo de 300 microsegundos dentro del semáforo.
- Límite de 3 hilos simultáneos.
- Comparación con `std::counting_semaphore`.
- Tiempo de espera de 50 ms sin permisos disponibles.

### Ejecución en Visual Studio

1. Seleccionar `prueba_semaforo.exe` como ejecutable.
2. Ejecutar mediante Ctrl + F5.
3. Comprobar que todas las pruebas muestran OK.

El resultado esperado es de 240 operaciones completadas con cada implementación, sin superar los 3 hilos simultáneos.

### Uso en el banco

El semáforo propio se utilizará para controlar los huecos disponibles de la cola antifraude, cuya capacidad es de 8 elementos.

## P5 - Integracion del banco

Se han integrado los hilos clientes, auditor,
nomina y antifraude.

El programa dispone de tres modos de fallo:

- carrera
- interbloqueo
- inanicion

El cierre de los hilos se realiza mediante join,
notificaciones de cierre y un valor centinela.

El proyecto compila correctamente.

Incidencias pendientes de integracion del equipo:

- Registro de los ingresos de la nomina.
- Notificacion del progreso al vigilante.
- Integracion completa de las operaciones de P2 y P4.

La ejecucion normal todavia no supera todas
las comprobaciones.