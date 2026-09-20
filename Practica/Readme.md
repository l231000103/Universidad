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

## Diagrama
Imagenes del Servomotor armado en Tinkercad y en físico.

<img src="Diagrama/Motorvoz.jpeg" width="320"><img src="Diagrama/armado.jfif" width="320"> 
## Código

Este programa convierte un Arduino UNO R4 WiFi en un servidor web controlado desde una app de MIT App Inventor. Por Wi-Fi recibe órdenes para encender o apagar un LED externo y la matriz LED de la placa. También controla un motor de CD con un puente H L298N, que puede avanzar o retroceder en tres velocidades o detenerse, y después de cada orden le responde a la app con un mensaje de confirmación.
[motorvoz.ino](codigos/motorvoz.ino)
- [Diseño y bloques de la aplicación (PDF)](codigos/App_Inventor/App_Inventor_CPV.pdf)
- [Archivo del proyecto (.aia)](codigos/App_Inventor/Control_Por_Voz.aia.zip)

> **Nota:** para que la aplicación funcione, el teléfono y el Arduino deben estar conectados a la misma red WiFi, y la variable `ip` debe tener la dirección que el Arduino muestre en el monitor serie.

## Video
El video muestra todo 

* [Readme](video/Readme.txt)
* [Ver video en YouTube](https://youtu.be/HVi7jjqaD8g?si=P1OTq7G0HO8tRpMl)

## Resultados

El motorreductor respondió a los comandos de voz enviados desde la app para avanzar, retroceder y detenerse. El sentido de giro se controla invirtiendo los pines 8 y 7 del puente H L293D, y la velocidad, con la señal PWM del pin 9:

| Nivel     | PWM (0–255) | Ciclo de trabajo | Comportamiento      |
|-----------|-------------|------------------|---------------------|
| Sin señal | 0           | 0 %              | Motor detenido      |
| Baja      | 110         | 43.1 %           | Umbral de arranque  |
| Media     | 180         | 70.6 %           | Giro intermedio     |
| Máxima    | 255         | 100 %            | Giro a plena marcha |

- Con valores de PWM menores a 110 el motor no gira, porque la fuerza no alcanza a vencer la fricción de los engranajes del reductor.
- Arriba de ese umbral, la velocidad aumenta con el PWM, y los tres niveles funcionan igual en avance y en retroceso.
- El motor necesita alimentación externa (pila de 9 V). Los 5 V del Arduino solo alimentan la lógica del L293D.
- El GND del Arduino, el negativo de la pila y las tierras del L293D deben ir a un negativo común; si no, el motor no responde.
- La app y el Arduino deben estar en la misma red Wi-Fi, con la IP correcta configurada en la app. Si el comando de voz no coincide con ninguna ruta, la app avisa que no entendió y no envía la petición.

## Reporte

[Reporte.pdf](Reporte/Reporte.pdf)

Este documento contiene introducción, la metodología utilizada, el análisis de los resultados y las conclusiones obtenidas durante la práctica.


## Conclusiones

La práctica permitió comprender cómo se controla un motor de corriente directa con Arduino por medio de un puente H: con salidas digitales (`digitalWrite`) se define el sentido de giro y con una señal PWM (`analogWrite`) se regula la velocidad, de modo que cada comando de voz se traduce en una acción del motor. También quedó clara la importancia de alimentar el motor con una fuente externa, ya que los pines del Arduino no pueden entregar la corriente que necesita, y de unir las tierras del Arduino, la pila y el L293D en un negativo común, porque sin esa referencia compartida el puente H no interpretaría correctamente las señales del Arduino.



