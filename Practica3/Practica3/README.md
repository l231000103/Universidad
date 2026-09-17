# Comparación de delay() y millis() con Arduino

Arduino Uno R4 WiFi · 3 LEDs · Temporización bloqueante vs. no bloqueante

## Descripción

Esta práctica compara dos formas de manejar tiempos en Arduino: la función `delay()`, que detiene por completo la ejecución del programa mientras espera, y la función `millis()`, que permite llevar el conteo del tiempo sin bloquear el resto del código. Para comparar ambas se usó el mismo circuito con tres LEDs (verde, amarillo y rojo), cada uno con un intervalo de tiempo distinto, programado primero con `delay()` y después con `millis()`.

## Objetivos de aprendizaje

- Controlar la temporización de varias salidas digitales con `delay()`.
- Controlar la temporización de varias salidas digitales de forma no bloqueante con `millis()`, usando la técnica de "tiempo anterior" (`unsigned long`) por cada LED.
- Comparar el comportamiento de un programa bloqueante contra uno no bloqueante cuando se manejan varios eventos con tiempos distintos al mismo tiempo.
- Usar el monitor serie para reportar un evento (encendido del LED rojo) sin depender de `delay()`.

## Material utilizado

- Arduino Uno R4 WiFi
- 3 LEDs (1 verde, 1 amarillo, 1 rojo)
- 3 resistencias de 330 Ω
- Protoboard
- Cables Dupont

## Diagrama del circuito

![Diagrama del circuito](Diagrama/diagrama_leds.png)

El mismo circuito se usó para las dos versiones del código (delay y millis).

### Conexiones

| Elemento | Pin del Arduino | Función |
|---|---|---|
| LED verde (ánodo, a través de resistencia de 330 Ω) | Pin 9 | Parpadea cada 500 ms |
| LED amarillo (ánodo, a través de resistencia de 330 Ω) | Pin 10 | Parpadea cada 1000 ms |
| LED rojo (ánodo, a través de resistencia de 330 Ω) | Pin 11 | Parpadea cada 1500 ms |
| Cátodo de los 3 LEDs | GND | Retorno común de corriente |

### Función de cada componente

- **Arduino Uno R4 WiFi:** ejecuta el programa y controla el encendido y apagado de los tres LEDs.
- **LEDs (verde, amarillo, rojo):** indicadores visuales, cada uno con su propio intervalo de tiempo.
- **Resistencias de 330 Ω:** limitan la corriente que pasa por cada LED para protegerlo.
- **Protoboard:** permite armar el circuito sin soldar los componentes.

## Código

- [Delay.ino](Codigo/Delay/Delay.ino): versión con `delay()`.
- [millis.ino](Codigo/Millis/millis.ino): versión con `millis()`.

### Versión con delay()

Enciende y apaga cada LED uno por uno, de forma secuencial: verde medio segundo, amarillo un segundo y rojo segundo y medio. Como `delay()` detiene todo el programa mientras espera, los LEDs nunca están encendidos al mismo tiempo ni parpadean de forma independiente entre sí: cada uno tiene que esperar su turno.

### Versión con millis()

Guarda en una variable (`tiempoVerde`, `tiempoAmarillo`, `tiempoRojo`) el último momento en que cada LED cambió de estado, y en cada vuelta del `loop()` compara `millis()` contra ese valor. Así, cada LED cambia de estado en su propio intervalo (500, 1000 y 1500 ms) sin esperar a los demás ni bloquear el programa, por lo que los tres pueden estar encendidos o parpadeando al mismo tiempo de forma independiente. Además, cada vez que el LED rojo se enciende, se envía por el monitor serie (9600 baudios) el mensaje `"Ximena la mas chambeadora"`.

## Terminal

Ver [Readme](Terminal/Readme.txt): la versión con `millis()` reporta por el monitor serie cada vez que se enciende el LED rojo; la versión con `delay()` no usa el monitor serie.

## Video del funcionamiento

- Versión con `delay()`: [Ver video en YouTube](https://youtu.be/zXMxCz9uoMA?si=Up6rGS999ETC38SV)
- Versión con `millis()`: [Ver video en YouTube](https://youtu.be/gIHIuMy8Fag?si=nVLI3jlfM2oAtlDP)

## Evidencias de armado

<img src="Imagenes/armado_1.png" width="320"> <img src="Imagenes/armado_2.png" width="320"> <img src="Imagenes/armado_3.png" width="320">

## Resultados

Incluye: [Readme](Resultados/Readme.txt)

Comparación entre las dos formas de trabajar, usando el mismo circuito y los mismos intervalos (500 ms, 1000 ms y 1500 ms):

| Aspecto | delay() | millis() |
|---|---|---|
| Ejecución | Bloqueante: detiene todo el programa mientras espera | No bloqueante: el `loop()` sigue corriendo mientras cuenta el tiempo |
| LEDs simultáneos | No; cada LED espera su turno, uno tras otro | Sí; los tres LEDs parpadean en su propio intervalo al mismo tiempo |
| Uso del monitor serie u otras tareas mientras se espera | No es posible | Sí es posible, como el mensaje que se envía al encender el LED rojo |
| Complejidad del código | Más simple | Requiere llevar el control del tiempo con variables por cada evento |

## Conclusiones

La práctica permitió comparar de forma directa dos maneras de manejar el tiempo en Arduino. Con `delay()` el programa es más sencillo de escribir, pero al detener por completo la ejecución impide que varios eventos ocurran de forma independiente: los tres LEDs terminan parpadeando uno después del otro en vez de simultáneamente. Con `millis()` el código es un poco más complejo, porque hay que llevar el registro del último cambio de cada LED en una variable, pero a cambio el programa nunca se detiene y cada LED respeta su propio intervalo sin afectar a los demás, lo que además permite hacer otras tareas al mismo tiempo, como enviar un mensaje por el monitor serie. Esto deja claro por qué `millis()` es la opción recomendada cuando un proyecto necesita manejar varios tiempos o eventos a la vez.

## Reporte

Ver [Readme](Reporte/Readme.txt)

Este documento contiene la descripción de la práctica, objetivos y procedimientos realizados, comparando las versiones con `delay()` y con `millis()`.

## Estructura de carpetas

```
Practica3/
├── README.md
├── Codigo/
│   ├── Delay/Delay.ino
│   └── Millis/millis.ino
├── Diagrama/                 ← esquemático (Tinkercad)
├── Imagenes/                 ← fotos del armado
├── Terminal/                 ← mensaje del monitor serie (versión millis)
├── Resultados/                ← comparación delay vs. millis
├── Reporte/                   ← reporte de la práctica
└── Video/                     ← enlaces a los videos (delay y millis)
```
