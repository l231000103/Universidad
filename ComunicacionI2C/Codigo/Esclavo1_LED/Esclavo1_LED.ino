// Practica: Comunicacion I2C entre 4 Arduinos
// ESCLAVO 1 (direccion 0x08): recibe una orden del maestro y enciende o apaga un LED.

#include <Wire.h>

const uint8_t PIN_LED = 13;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Wire.begin(0x08);           // direccion de este esclavo
  Wire.onReceive(recibir);
}

void loop() {
}

void recibir(int cuantos) {
  while (Wire.available()) {
    byte dato = Wire.read();
    digitalWrite(PIN_LED, dato ? HIGH : LOW);
  }
}
