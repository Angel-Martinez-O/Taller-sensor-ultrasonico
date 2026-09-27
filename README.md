# Práctica — Sensor ultrasónico HC-SR04

## Integrantes

- Nombre completo: Angel Martinez, Jesus Aguilar, Andres Bruges
- Grupo: 3A
- Asignatura: Fundamentos de Mecatrónica

## Objetivo

En esta práctica conectamos un sensor ultrasónico HC-SR04 a un Arduino UNO para medir distancias y mostrarlas en el Monitor Serie. Luego, usando lo aprendido, desarrollamos un contador de objetos que detecta cuándo pasa algo frente al sensor a menos de 10 cm, sin contar el mismo objeto varias veces mientras permanece ahí.

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

El HC-SR04 mide la distancia enviando un pulso ultrasónico y midiendo cuánto tarda en regresar el eco. El pin **TRIG** es el que activamos durante 10 microsegundos para que el sensor emita el sonido. El pin **ECHO** permanece en alto durante el tiempo que tarda la onda en ir hasta el objeto y regresar.

Con `pulseIn(echoPin, HIGH)` obtenemos ese tiempo en microsegundos y luego calculamos la distancia con la siguiente fórmula:

```text
distancia = duracion * 0.0343 / 2
```

Multiplicamos por la velocidad del sonido y dividimos entre 2 porque el sonido realiza un recorrido de ida y vuelta, pero nosotros necesitamos conocer solamente la distancia hasta el objeto.

Durante las pruebas observamos que mientras más cerca colocábamos un objeto del sensor, menor era el valor que aparecía en el Monitor Serie. Al alejar el objeto, el valor aumentaba. Por ejemplo, obtuvimos mediciones de 41.8 cm cuando el objeto estaba más lejos y 22.4 cm cuando lo acercamos.

## Resultados de las mediciones

| Distancia real | Distancia medida |
|----------------:|------------------:|
| 10 cm            | 10.39 cm          |
| 20 cm            | 20.72 cm          |
| 30 cm            | 30.75 cm          |
| 40 cm            | 40.10 cm          |
| 50 cm            | 50.13 cm          |

## Reto — Contador de objetos

Para realizar el contador tuvimos que pensar en dos estados: **esperando objeto** y **objeto presente**. Esto fue necesario porque si solamente comparábamos si la distancia era menor de 10 cm, el mismo objeto se podía contar varias veces mientras permaneciera quieto frente al sensor.

Para solucionar esto utilizamos una variable booleana llamada `objetoPresente` y una variable entera llamada `totalObjetos`:

```cpp
int totalObjetos = 0;
bool objetoPresente = false;
```

La lógica que utilizamos fue la siguiente:

- Si la distancia es menor a 10 cm y mayor a 0, y `objetoPresente` está en `false`, significa que acaba de entrar un objeto nuevo. En ese momento aumentamos `totalObjetos` en 1, mostramos **"Objeto detectado"** y cambiamos `objetoPresente` a `true`.
- Mientras el objeto continúe a menos de 10 cm, `objetoPresente` permanece en `true`, por lo que el programa no vuelve a contar el mismo objeto.
- Cuando la distancia vuelve a ser mayor o igual a 10 cm, cambiamos `objetoPresente` nuevamente a `false`. De esta manera, el sistema queda preparado para detectar el siguiente objeto.

Para mostrar el mensaje **"LOTE COMPLETADO"** utilizamos el operador módulo. Cada vez que `totalObjetos % 5 == 0`, significa que se completó un grupo de 5 objetos y mostramos el mensaje en el Monitor Serie.

También agregamos un `delay(50)` al final del `loop()` para que las lecturas fueran más estables durante la simulación.

En resumen, el contador solamente aumenta cuando detectamos la entrada de un objeto nuevo. Si el objeto permanece frente al sensor, no se vuelve a contar.

Finalmente, probamos el sistema con varios objetos consecutivos, acercando cada uno a menos de 10 cm y retirándolo antes de colocar el siguiente. El conteo aumentó correctamente de uno en uno sin duplicar los objetos y, al llegar al quinto objeto, apareció el mensaje **"LOTE COMPLETADO"**, como se puede observar en las evidencias de la práctica.
