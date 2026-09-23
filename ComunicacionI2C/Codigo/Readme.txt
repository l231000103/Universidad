Codigo de la practica (un sketch por cada Arduino UNO R4 WiFi):

Maestro/Maestro.ino                               -> maestro del bus I2C (sin direccion). Pide el potenciometro cada 500 ms, manda el angulo al servo y recibe 1/0 por el Monitor serie para el LED.
Esclavo1_LED/Esclavo1_LED.ino                     -> esclavo 0x08, enciende o apaga el LED (pin 13).
Esclavo2_Servo/Esclavo2_Servo.ino                 -> esclavo 0x09, mueve el servomotor (pin 9) al angulo recibido.
Esclavo3_Potenciometro/Esclavo3_Potenciometro.ino -> esclavo 0x0A, lee el potenciometro (A0) y lo envia en 2 bytes.

Nota: el maestro incluye la linea "while (!Serial && millis() < 3000)" que necesita el R4 WiFi para no perder los primeros mensajes del Monitor serie.
