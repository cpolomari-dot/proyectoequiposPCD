
# EXPERIMENTOS - Hito 1
## Caso 3: Banco

### Experimento 1: Funcionamiento del semaforo

**Prediccion:**

Se espera que el semaforo bloquee los hilos cuando
su contador sea igual a cero.

Cuando otro hilo ejecute release(), el semaforo
incrementara su contador y permitira continuar
a uno de los hilos bloqueados.

**Resultado observado:**

La prueba prueba_semaforo se ejecuto correctamente
y mostro el resultado OK.

### Experimento 2: Race Condition en Banco

**Prediccion:**

Varios clientes modificaran simultaneamente
el saldo de una cuenta bancaria compartida
sin utilizar un mecanismo de sincronizacion.

Esto puede provocar la perdida de actualizaciones.

**Configuracion:**

- 8 clientes.
- 5000 operaciones por cliente.
- 1 unidad por operacion.
- Saldo inicial: 0.
- Saldo esperado: 40000.
- 10 repeticiones.

**Resultado observado:**

Las 10 ejecuciones presentaron invariantes rotos.

Los saldos finales fueron inferiores al esperado:

- Ejecucion 1: 38308
- Ejecucion 2: 38125
- Ejecucion 3: 38058
- Ejecucion 4: 38214
- Ejecucion 5: 38195
- Ejecucion 6: 38026
- Ejecucion 7: 38049
- Ejecucion 8: 37900
- Ejecucion 9: 38236
- Ejecucion 10: 37760

### Experimento 3: Ejecucion con sincronizacion

**Prediccion:**

El uso de un mutex protegera las operaciones
sobre el saldo compartido.

**Resultado observado:**

El saldo final fue 40000, coincidiendo
con el resultado esperado.

No se detectaron invariantes rotos.

### Conclusion

Los experimentos muestran que la ausencia
de sincronizacion puede provocar perdida
de actualizaciones en el saldo bancario.

El uso de un mutex permite proteger
las operaciones concurrentes y mantener
la consistencia del saldo.
