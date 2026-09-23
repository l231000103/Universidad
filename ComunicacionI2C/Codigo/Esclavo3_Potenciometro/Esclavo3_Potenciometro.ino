// Practica: Comunicacion I2C entre 4 Arduinos
// ESCLAVO 3 (direccion 0x0A): lee un potenciometro y envia su valor al maestro cuando este se lo pide.

#include <Wire.h>

void setup() {
  Wire.begin(0x0A);
  Wire.onRequest(enviar);
}

void loop() {
}

void enviar() {
  int valor = analogRead(A0);      // 0 a 1023
  Wire.write(highByte(valor));     // se manda en 2 bytes
  Wire.write(lowByte(valor));
}
