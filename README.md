# Práctica — Sensor ultrasónico HC-SR04

## Integrantes

- Nombre completo: Angel Martinez , Jesus Aguilar ,  Andres Bruges 
- Grupo: [3A]
- Asignatura: [Fundamentos de mecatronica]

## Objetivo

En esta práctica conecté un sensor ultrasónico HC-SR04 a un Arduino UNO para medir distancias y mostrarlas en el Monitor Serie. Luego, usando lo aprendido, desarrollé un contador de objetos que detecta cuándo pasa algo frente al sensor a menos de 10 cm, sin contar el mismo objeto varias veces mientras permanece ahí.

## Materiales

- Arduino UNO
- Sensor ultrasónico HC-SR04
- Protoboard
- Cables Dupont
- Cable USB
- Computador con Arduino IDE

## Conexiones

| HC-SR04 | Arduino |
|---------|---------|
| VCC     | 5V      |
| GND     | GND     |
| TRIG    | D9      |
| ECHO    | D10     |

## Funcionamiento

El HC-SR04 mide distancia mandando un pulso ultrasónico y midiendo cuánto tarda en volver el eco. El pin **TRIG** es el que yo activo en alto por 10 microsegundos para que el sensor emita el sonido. El pin **ECHO** se pone en alto todo el tiempo que la onda tarda en ir hasta el objeto y regresar, y con `pulseIn(echoPin, HIGH)` obtengo justo esa duración en microsegundos.

Con ese tiempo calculo la distancia así:


distancia = duracion * 0.0343 / 2


Multiplico por la velocidad del sonido en cm/µs y divido entre 2 porque el tiempo medido corresponde al viaje de ida **y** vuelta del sonido, pero la distancia que quiero es solo la de ida (hasta el objeto).

Al probarlo, mientras más cerca ponía un objeto del sensor, más pequeño era el número de centímetros que mostraba el Monitor Serie, y al alejarlo el valor subía, tal como se ve en las capturas de evidencia (por ejemplo 41.8 cm con el objeto lejos y 22.4 cm al acercarlo).

## Resultados de las mediciones

| Distancia real | Distancia medida |
|----------------:|------------------:|
| 10 cm            | 10.39 cm           |
| 20 cm            | 20.72 cm           |
| 30 cm            | 30.75 cm           |
| 40 cm            | 40.10 cm           |
| 50 cm            | 50.13 cm           |

## Reto — Contador de objetos

Para el contador tuve que pensar el problema en dos estados: **esperando objeto** y **objeto presente**, porque si solo comparaba "¿está a menos de 10 cm?" en cada vuelta del `loop()`, el mismo objeto se iba a contar decenas de veces mientras estuviera quieto frente al sensor (el programa corre muchas veces por segundo).

La solución que usé fue guardar el estado con una variable booleana `objetoPresente` y el conteo en un entero `totalObjetos`:


int totalObjetos = 0;
bool objetoPresente = false;


La lógica es la siguiente:

- Si la distancia es menor a 10 cm (y mayor a 0, para descartar lecturas raras del sensor) **y** `objetoPresente` todavía está en `false`, quiere decir que acaba de **entrar** un objeto nuevo: ahí sumo 1 a `totalObjetos`, imprimo "Objeto detectado" y cambio `objetoPresente` a `true`.
- Mientras el objeto siga a menos de 10 cm, `objetoPresente` ya está en `true`, así que no vuelvo a sumar aunque el `loop()` se siga ejecutando.
- Cuando la distancia vuelve a ser mayor o igual a 10 cm, pongo `objetoPresente` de nuevo en `false`, para que el sistema quede listo para detectar el siguiente objeto.

Para el "LOTE COMPLETADO" no usé un contador aparte, sino el operador módulo: cada vez que `totalObjetos % 5 == 0`, sé que acabo de completar un múltiplo de 5 y muestro el mensaje. También le agregué un `delay(50)` al final del `loop()` para que las lecturas del simulador sean más estables.

O sea, el contador solo sube en la **transición** de "esperando objeto" a "objeto presente", no mientras el objeto se queda ahí.

Además, agregué que cada vez que `totalObjetos` llega a un múltiplo de 5, el Monitor Serie muestre "LOTE COMPLETADO".

Al probarlo con varios objetos consecutivos (acercando cada uno a menos de 10 cm, dejándolo unos segundos y retirándolo antes del siguiente), el conteo subió correctamente de uno en uno sin duplicarse, y al llegar al quinto objeto apareció el mensaje de lote completado, como se ve en la evidencia:


Objeto detectado
Total de objetos: 3
Objeto detectado
Total de objetos: 4
Objeto detectado
Total de objetos: 5
LOTE COMPLETADO
