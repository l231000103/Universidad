Codigo de la practica:

Inclinometro_IMU/Inclinometro_IMU.ino -> sketch para Arduino UNO R4 WiFi. Lee el MPU-6050 por I2C (registros directos, sin biblioteca), calcula el pitch con filtro complementario y controla el motorreductor con el L298N (ENA=D9, IN1=D8, IN2=D7). Tiene zona muerta de 5 grados, rampa de aceleracion, paro de seguridad si falla el sensor, indicador en la matriz LED y mensajes por Monitor serie a 115200 baudios.

Nota: el sensor debe permanecer QUIETO durante la calibracion del giroscopio (500 muestras, unos 5 segundos) al encender o pulsar RESET.
