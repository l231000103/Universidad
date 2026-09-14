# Control de motorreductor por voz con Arduino y puente H L293D

## Descripción

El objetivo de esta práctica es controlar un motorreductor por medio de comandos de voz. El Arduino Uno R4 WiFi recibe la orden y, a través del puente H L293D, hace que el motor avance, retroceda o se detenga.

## Objetivos de aprendizaje

Programar y simular en Arduino el control de un motorreductor de corriente directa usando un puente H (L293D): definir el sentido de giro con las salidas digitales de los pines 7 y 8 (`digitalWrite()`) y regular la velocidad con una señal PWM en el pin 9 (`analogWrite()`), para que el motor avance, retroceda o se detenga según el comando de voz recibido.

## Material utilizado

* Arduino Uno R4 WiFi
* Puente H (en la simulación de Tinkercad se usó el circuito integrado L293D)
* Motorreductor
* Pila de 9 V
* Protoboard
* Cables Dupont

## Diagrama del circuito

![Diagrama del circuito](Practica/Diagrama/Motorvoz.jpeg)

### Conexiones

| Elemento | Pata del L293D | Función |
|---|---|---|
| 5 V del Arduino | 16 (VCC1) | Alimenta la parte del L293D que recibe las órdenes |
| Positivo (+) de la pila de 9 V | 8 (VCC2) | Alimenta el motor |
| Pin 9 del Arduino | 1 (Enable 1) | Controla la velocidad del motor (PWM) y permite detenerlo |
| Pin 8 del Arduino | 2 (Entrada 1) | Junto con el pin 7, define el sentido de giro |
| Pin 7 del Arduino | 7 (Entrada 2) | Junto con el pin 8, define el sentido de giro |
| Terminales del motor | 3 y 6 (Salidas 1 y 2) | Una terminal en cada pata; por ahí llega la energía al motor |
| GND del Arduino y negativo (−) de la pila | 4, 5, 12 y 13 (GND) | Tierra común para que todo comparta la misma referencia eléctrica |
| Negativo común (GND) | 9, 10 y 15 | Mantienen apagado el segundo canal del L293D, que no se usa |
| Sin conectar | 11 y 14 | Salidas del segundo canal |

### Función de cada componente

* **Arduino Uno R4 WiFi:** ejecuta el programa y envía las órdenes para que el motor avance, retroceda o se detenga.
* **Puente H L293D:** recibe las órdenes del Arduino y controla la energía que llega al motor para cambiar su velocidad y sentido de giro.
* **Motorreductor:** convierte la energía eléctrica en movimiento. Sus engranajes reducen la velocidad y aumentan la fuerza de giro.
* **Pila de 9 V:** proporciona la energía para mover el motor.
* **Protoboard:** permite unir los componentes con cables sin soldarlos.

### Tabla de funcionamiento

| Acción | Pin 9 (pata 1) | Pin 8 (pata 2) | Pin 7 (pata 7) |
|---|---|---|---|
| Avanzar | PWM | HIGH | LOW |
| Retroceder | PWM | LOW | HIGH |
| Detener | 0 (LOW) | Cualquiera | Cualquiera |

El valor PWM del pin 9 va de 0 a 255: cuanto más alto, más rápido gira el motor. El sentido que corresponde a "avanzar" depende de cómo se conectaron las terminales del motor en las patas 3 y 6; si gira al revés, basta con intercambiarlas.

## Código

[motorvoz.ino](Practica/codigos/motorvoz.ino)

## Terminal
[Monitor_Serial.png](Practica/terminal/Monitor_Serial.png)

## Aplicación en App Inventor

La aplicación móvil es la que recibe los comandos de voz. Fue desarrollada en MIT App Inventor y funciona como control remoto del Arduino: reconoce lo que dice el usuario, lo compara con una lista de comandos y envía la orden correspondiente al Arduino por medio de una petición HTTP a través de la red WiFi.

### Componentes utilizados

| Componente | Tipo | Función |
|---|---|---|
| `BtnHablar` | Button | Inicia el reconocimiento de voz al presionarlo |
| `LblEstado` | Label | Muestra la respuesta de la acción ejecutada |
| `LblComando` | Label | Muestra el texto reconocido y la ruta enviada |
| `Voz` | SpeechRecognizer | Convierte la voz del usuario en texto |
| `Habla` | TextToSpeech | Confirma por voz la acción realizada |
| `Web1` | Web | Envía la petición HTTP al Arduino |

### Funcionamiento de los bloques

La aplicación guarda en la variable global `ip` la dirección del Arduino (`http://192.168.100.42`) y en `conectado` el estado de la conexión.

Al presionar **BtnHablar** se llama a `Voz.GetText`, que abre el reconocimiento de voz del teléfono. Cuando termina, el bloque `Voz.AfterGettingText` recibe el texto y lo compara con cada comando usando el bloque `contains`. Si alguno coincide, se llama al procedimiento `enviar`.

El procedimiento `enviar` recibe dos parámetros, `ruta` y `respuesta`, y realiza cuatro acciones:

1. Muestra la respuesta en `LblEstado`.
2. La dice en voz alta con `Habla.Speak`.
3. Arma la dirección completa uniendo la variable global `ip` con la ruta recibida.
4. Envía la petición al Arduino con `Web1.Get`.

Si el texto reconocido no coincide con ningún comando, el bloque `else` responde por voz **"No te entendí"** y no se envía ninguna petición.

### Comandos de voz reconocidos

| Comando de voz | Ruta enviada | Respuesta de la app |
|---|---|---|
| prende el LED | `/led/on` | LED encendido |
| apaga el LED | `/led/off` | LED apagado |
| prende la matriz | `/matriz/on` | Matriz encendida |
| apaga la matriz | `/matriz/off` | Matriz apagada |
| avanza velocidad baja | `/motor/avanza/baja` | Avanzando velocidad baja |
| avanza velocidad media | `/motor/avanza/media` | Avanzando velocidad media |
| avanza velocidad máxima | `/motor/avanza/maxima` | Avanzando velocidad maxima |
| retrocede velocidad baja | `/motor/retrocede/baja` | Retrocediendo velocidad baja |
| retrocede velocidad media | `/motor/retrocede/media` | Retrocediendo velocidad media |
| retrocede velocidad máxima | `/motor/retrocede/maxima` | Retrocediendo velocidad maxima |
| detener motor | `/motor/detener` | motor detenido |

### Archivos

- [Diseño y bloques de la aplicación (PDF)](Practica/codigos/App_Inventor/App_Inventor_CPV.pdf)
- [Archivo del proyecto (.aia)](Practica/codigos/App_Inventor/Control_Por_Voz.aia.zip)

> **Nota:** para que la aplicación funcione, el teléfono y el Arduino deben estar conectados a la misma red WiFi, y la variable `ip` debe tener la dirección que el Arduino muestre en el monitor serie.

## Video del funcionamiento

* [Readme](Practica/Video/Readme.txt)
* [Ver video en YouTube](https://youtu.be/HVi7jjqaD8g?si=P1OTq7G0HO8tRpMl)

## Evidencias de armado

![Evidencia de armado](Practica/Diagrama/armado.jfif)

## Resultados

Incluye: [Resultados.pdf](Practica/Resultados/Resultados.pdf)

* Gráficas (si aplica)
* Tablas de datos
* Observaciones sobre el comportamiento del sistema

## Conclusiones

La práctica permitió comprender cómo se controla un motor de corriente directa con Arduino por medio de un puente H: con salidas digitales (`digitalWrite`) se define el sentido de giro y con una señal PWM (`analogWrite`) se regula la velocidad, de modo que cada comando de voz se traduce en una acción del motor. También quedó clara la importancia de alimentar el motor con una fuente externa, ya que los pines del Arduino no pueden entregar la corriente que necesita, y de unir las tierras del Arduino, la pila y el L293D en un negativo común, porque sin esa referencia compartida el puente H no interpretaría correctamente las señales del Arduino.

## Reporte

[Reporte.pdf](Practica/Reporte/Reporte.pdf)

Este documento contiene la descripción de la práctica, objetivos y procedimientos realizados.

* Reporte técnico estilo IEEE (PDF)
* Datos CSV (si aplica)
* Diagramas adicionales
