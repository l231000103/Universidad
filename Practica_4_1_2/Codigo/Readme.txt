Codigo de la practica:

Practica_4_1_2/Practica_4_1_2.ino -> version final para Arduino UNO R4 WiFi. Clase Boton con filtro de rebotes de 25 ms (millis()),
                                     estado seguro en setup(), aviso de reinicio y estado del motor en la matriz LED, rampa de PWM
                                     de 0 a 255 en 1.5 s y paro inmediato. Sin delay().
Practica_4_1_2_Tinkercad/Practica_4_1_2_Tinkercad.ino -> misma logica para simular en Tinkercad. No hay matriz LED, asi que el LED D13
                                     parpadea en el aviso de reinicio y queda fijo con el motor encendido.
Practica_4_1_2_SinFiltro/Practica_4_1_2_SinFiltro.ino -> paso 2: cuenta cada paso de suelto a oprimido leyendo el boton directo, sin filtro.

Librerias: la matriz usa Arduino_LED_Matrix.h, que viene con el paquete de la placa Arduino UNO R4 Boards (no hay que instalar nada).

IMPORTANTE: el Monitor Serie debe estar a 9600 baudios.

Ajustes en el codigo:
- VENTANA_MS: ventana de estabilidad del filtro (25 ms). Cambiarla a 2 y a 300 para el paso 6.
- RAMPA_MS: duracion de la rampa (1500 ms).
- AVISO_MS: cuanto dura el aviso de reinicio (2000 ms).
