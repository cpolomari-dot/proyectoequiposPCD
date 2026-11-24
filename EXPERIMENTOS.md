## P5 — Predicción del experimento

Antes de realizar las pruebas, considero que el periodo del auditor afectará al número de comprobaciones realizadas y a la competencia por los mutex de las cuentas.

- **Periodo de 2 ms:** espero que el auditor intente realizar más comprobaciones y que exista mayor competencia por los recursos.
- **Periodo de 20 ms:** espero un comportamiento intermedio entre las comprobaciones del auditor y las operaciones de los clientes.
- **Periodo de 200 ms:** espero que se realicen menos comprobaciones y que el auditor interfiera menos en las operaciones.

Con el modo de fallo `inanicion`, el auditor intentará adquirir los mutex mediante `try_lock`. Si alguna cuenta está ocupada, abandonará la pasada y volverá a intentarlo después de 1 ms.

Mi predicción es que, cuando exista mucha competencia entre los clientes, aumentará el número de pasadas fallidas del auditor.

Estas predicciones se comprobarán posteriormente mediante ejecuciones reales del programa.