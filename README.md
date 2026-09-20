# Sistemas Programables — Prácticas con Arduino UNO R4 WiFi

**Instituto Tecnológico de Mazatlán**
**Carrera:** Ingeniería en Sistemas Computacionales
**Materia:** Sistemas Programables
**Profesor:** Miguel Barrón  Hernández 
**Integrantes:** Ximena Armenta Dávila, Carlos Tadeo Ibarra Castañeda, Dulce Princesa Ulivarría González

Repositorio de prácticas de la materia, todas desarrolladas sobre un Arduino UNO R4 WiFi.

## Prácticas

| # | Práctica | Carpeta | Descripción |
|---|----------|---------|-------------|
| 1 | Motorreductor por comandos de voz | [`Practica/`](./Practica) | Control de un motorreductor con puente H (L293D) desde una app de MIT App Inventor con reconocimiento de voz, vía HTTP sobre WiFi. |
| 2 | Servomotor por interfaz web | [`Servomotor/`](./Servomotor) | Control de un servomotor MG996R desde una página web servida por el propio Arduino (control deslizante 0°–180°). |
| 3 | Comparación de `delay()` y `millis()` | [`Practica3/`](./Practica3) | Mismo circuito de 3 LEDs controlado con espera bloqueante (`delay()`) vs. no bloqueante (`millis()`), con mensajes por monitor serie. |
| 4 | Semáforo peatonal con botón | [`Semaforo/`](./Semaforo) | Semáforo vehicular y peatonal con solicitud de cruce por pulsador, temporizado con `millis()`. |
| 5 | Buffer circular con interrupción externa | [`BufferCircular/`](./BufferCircular) | Registro de eventos de un pulsador (interrupción en D2) en un arreglo circular de 32 posiciones, con antirrebote y matriz LED en animación simultánea. |

## Estructura de cada práctica

Cada carpeta sigue, en general, esta organización:

```
PracticaN/
├── Codigo/        # Sketch(es) .ino
├── Diagrama/       # Diagrama esquemático (Tinkercad / Fritzing)
├── Reporte.pdf      # Reporte formal (introducción, desarrollo, resultados, conclusiones)
└── Video/          # Video de la práctica en funcionamiento
```

## Herramientas comunes

- Arduino UNO R4 WiFi
- Arduino IDE 2
- Tinkercad Circuits (diagramas esquemáticos)
