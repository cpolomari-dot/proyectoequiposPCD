## P5 — Manuel Alcantud

### Uso de inteligencia artificial

Durante el desarrollo de mi parte del proyecto he utilizado ChatGPT como herramienta de apoyo para resolver dudas, obtener propuestas de código y comprender mejor los conceptos de programación concurrente.

Principalmente, me ha ayudado con:

- La implementación de los métodos del semáforo.
- La preparación de las pruebas con varios hilos.
- La comprensión de los mutex y las variables de condición.
- Los comentarios y las explicaciones del código.
- La configuración del programa principal del banco.
- La utilización de Git y GitHub.

He incorporado el código de forma progresiva en Visual Studio, realizando las pruebas y subiendo los avances mediante mis propios commits.

El objetivo de utilizar esta herramienta ha sido facilitar el desarrollo de mi parte del proyecto y comprender el funcionamiento del código para poder explicarlo durante la defensa.


### Registro de ayuda recibida

**Miembro:** Manuel Alcantud (P5)  
**Herramienta:** ChatGPT  
**Fechas:** 23 y 24 de noviembre de 2026.

Durante el desarrollo he utilizado ChatGPT para obtener propuestas de implementación, resolver dudas y comprender el funcionamiento del código. He ido incorporando los cambios en Visual Studio y registrando los avances en mi rama personal `p5-manuel`.

### Relación de aportaciones y commits

| Trabajo realizado con apoyo de IA | Commit |
|---|---|
| Configuración de `.gitignore` | `6eb79c9` |
| Implementación de `acquire()` | `65c995d` |
| Implementación de `release()` | `1b8039b` |
| Implementación de `tryAcquireFor()` | `7f2993d` |
| Primera prueba del semáforo | `a578377` |
| Prueba de devolución de permisos | `132c339` |
| Prueba de bloqueo y desbloqueo | `3ab50c0` |
| Prueba de concurrencia con 10 hilos | `14b302f` |
| Configuración inicial del banco | `a7c4f68` |
| Configuración de las seis claves | `6f13888` |
| Adaptación a 12 hilos y 20 repeticiones | `80aa403` |
| Prueba con `counting_semaphore` | `ab0c8bd` |
| Documentación del monitor | `3889832` |
| Documentación del semáforo en README | `9461944` |

### Correcciones realizadas

Durante el desarrollo se ha adaptado la prueba inicial de concurrencia a los requisitos exactos del enunciado, pasando de 10 hilos a 12 e incorporando 20 repeticiones por hilo.

También se ha añadido la comparación con el semáforo estándar de C++.

Se intentó integrar el trabajo de los compañeros, pero apareció un conflicto en `caso/main.cpp`. Se canceló temporalmente la integración para conservar las aportaciones de cada integrante.

### Valoración personal

La inteligencia artificial me ha servido como apoyo durante la práctica, tanto para desarrollar el código como para comprenderlo. He trabajado de forma progresiva, incorporando los cambios y realizando los commits individualmente.

### Integración final de P5 - 24/11/2026

Se ha utilizado ChatGPT como apoyo para la integración del código, la resolución de errores y la preparación de las pruebas.

Se han incorporado las aportaciones de los compañeros y se ha comprobado que el proyecto compila correctamente, quedando pendientes algunas incidencias de funcionamiento del equipo.