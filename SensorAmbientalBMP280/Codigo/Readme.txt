Codigo de la practica:

estacion_bmp280/estacion_bmp280.ino -> lee temperatura, presion y altitud del BMP280 por I2C cada 1000 ms (millis()),
                                       las manda al Monitor Serie / Serial Plotter y alterna temperatura (C) y altitud (m)
                                       en la matriz LED 12x8 cada 2500 ms. Si el sensor no responde muestra "ERR" y lo
                                       vuelve a buscar cada 5000 ms.

Librerias (Gestor de librerias): Adafruit BMP280 Library, Adafruit Unified Sensor y ArduinoGraphics.
Arduino_LED_Matrix ya viene incluida con la placa UNO R4.

IMPORTANTE: el Monitor Serie y el Serial Plotter deben estar a 115200 baudios. Si estan a otra velocidad
(por ejemplo 9600) solo aparecen simbolos raros.

Ajustes en el codigo:
- SEALEVEL_HPA: presion a nivel del mar del dia (1005.0 hPa). Cambiarla mejora la altitud calculada.
- USE_QWIIC: 0 = pines SDA/SCL (Wire), 1 = conector Qwiic (Wire1).
- Si el Chip ID es 0x60 el sensor es un BME280 y hay que usar el sketch con la libreria Adafruit_BME280.
