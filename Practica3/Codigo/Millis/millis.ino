const int ledVerde = 9;
const int ledAmarillo = 10;
const int ledRojo = 11;

unsigned long tiempoVerde = 0;
unsigned long tiempoAmarillo = 0;
unsigned long tiempoRojo = 0;

bool estadoVerde = false;
bool estadoAmarillo = false;
bool estadoRojo = false;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);

  Serial.begin(9600);
}

void loop() {
  unsigned long tiempoActual = millis();

  // Verde: cambia de estado cada 500 milisegundos.
  if (tiempoActual - tiempoVerde >= 500) {
    tiempoVerde = tiempoActual;
    estadoVerde = !estadoVerde;
    digitalWrite(ledVerde, estadoVerde);
  }

  // Amarillo: cambia de estado cada 1000 milisegundos.
  if (tiempoActual - tiempoAmarillo >= 1000) {
    tiempoAmarillo = tiempoActual;
    estadoAmarillo = !estadoAmarillo;
    digitalWrite(ledAmarillo, estadoAmarillo);
  }

  // Rojo: cambia de estado cada 1500 milisegundos.
  if (tiempoActual - tiempoRojo >= 1500) {
    tiempoRojo = tiempoActual;
    estadoRojo = !estadoRojo;
    digitalWrite(ledRojo, estadoRojo);

    // Envía el mensaje solamente cuando se enciende.
    if (estadoRojo) {
      Serial.println("Ximena la mas chambeadora");
    }
  }
}