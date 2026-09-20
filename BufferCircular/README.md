# Buffer circular con interrupción externa

Arduino Uno R4 WiFi · Matriz LED integrada · Pulsador en pin de interrupción (D2)

## Descripción

El proyecto simula el sensor de piezas de una banda transportadora: cada vez que se presiona el pulsador, el Arduino debe registrar ese evento sin perderlo, aunque en ese momento esté ocupado dibujando una animación en su matriz de LEDs. El pulsador se conecta a un pin capaz de generar una **interrupción externa**, así que en vez de tener que revisarlo constantemente en el `loop()`, le avisa al Arduino de inmediato en el instante en que se presiona, sin importar qué parte del código esté ejecutando. Para no perder eventos ni bloquear el programa, cada aviso se guarda temporalmente en un **buffer circular** de tamaño fijo (32 posiciones): la interrupción solo anota el instante del evento y sale, mientras que el `loop()` los va leyendo y procesando a su propio ritmo, sin dejar de animar la matriz LED.

## Objetivos de aprendizaje

- Configurar una interrupción externa (modo `FALLING`) en un pin del Arduino para reaccionar a un evento impredecible sin tener que revisarlo constantemente desde el `loop()`.
- Implementar un buffer circular de tamaño fijo para guardar eventos generados por la interrupción sin perder ninguno, usando índices de escritura y lectura independientes.
- Distinguir el estado "buffer lleno" del estado "buffer vacío" cuando ambos índices podrían apuntar al mismo lugar.
- Aplicar un filtro de antirrebote (40 ms) dentro de la propia rutina de interrupción, en vez de dentro del `loop()`.
- Mantener una animación no bloqueante en la matriz LED con `millis()` mientras se atienden los eventos registrados por la interrupción.

## Material utilizado

- Arduino Uno R4 WiFi (con matriz LED integrada)
- Pulsador de cuatro patas
- Protoboard
- 2 cables de conexión
- Cable USB-C

## Diagrama del circuito

![Diagrama del circuito](Diagrama/diagrama_buffer.png)
<img src="Diagrama/armado_1.png" width="320"> <img src="Diagrama/armado_2.png" width="320">

## Código

[Buffer_Circular1.ino](Codigo/Buffer_Circular1.ino)

## Video del funcionamiento

[Ver video en YouTube](https://youtu.be/Lg2vXyP2IQk)


## Resultados

Incluye: [Resultados_buffercircular.pdf](Resultados/Resultados_buffercircular.pdf). Contiene las gráficas del conteo acumulado y del intervalo entre los 30 eventos registrados, las tablas de indicadores calculados y de los primeros diez eventos, y las observaciones sobre el comportamiento del sistema.

## Reporte


Incluye: [Reporte_buffercircular.pdf](Reporte/Reporte_buffercircular.pdf). 
## Conclusiones

La práctica permitió registrar eventos impredecibles sin perder ninguno, combinando una interrupción externa (pin D2, modo `FALLING`) con un buffer circular de 32 posiciones. La interrupción se limitó a anotar el instante de cada pulsación y salir de inmediato, mientras que el `loop()` procesó esos datos a su propio ritmo y mantuvo la animación de la matriz LED en movimiento constante, sin usar `delay()` en ningún punto del programa. El filtro de antirrebote (40 ms) dentro de la propia interrupción evitó que un solo clic físico se registrara como varias pulsaciones, y la variable `eventosPendientes` resolvió la ambigüedad entre el buffer lleno y el vacío cuando ambos índices podrían coincidir. Las pruebas confirmaron el orden y la integridad de los 30 eventos registrados, aunque quedó pendiente demostrar la vuelta completa del buffer y su respuesta al llenarse, algo que requeriría una prueba con más de 32 pulsaciones.

