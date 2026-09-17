const int rojoPeaton = 12;
const int verdePeaton = 11;
const int rojoAutos = 10;
const int amarilloAutos = 9;
const int verdeAutos = 8;
const int boton = 2;

// 0 = verde, 1 = amarillo, 2 = rojo.
int estado = 0;
bool solicitudPeaton = false;
unsigned long inicioEstado = 0;

// Evitar lecturas repetidas por los rebotes del boton.
int lecturaAnterior = HIGH;
int estadoBoton = HIGH;
unsigned long ultimoCambioBoton = 0;
const unsigned long antirrebote = 40;

void setup() {
  pinMode(rojoPeaton, OUTPUT);
  pinMode(verdePeaton, OUTPUT);
  pinMode(rojoAutos, OUTPUT);
  pinMode(amarilloAutos, OUTPUT);
  pinMode(verdeAutos, OUTPUT);
  pinMode(boton, INPUT_PULLUP);

  digitalWrite(verdePeaton, LOW);
  digitalWrite(rojoPeaton, HIGH);
  digitalWrite(rojoAutos, LOW);
  digitalWrite(amarilloAutos, LOW);
  digitalWrite(verdeAutos, HIGH);

  inicioEstado = millis();
}

void loop() {
  unsigned long ahora = millis();
  int lectura = digitalRead(boton);

  // Leer el boton continuamente.
  if (lectura != lecturaAnterior) {
    ultimoCambioBoton = ahora;
  }

  if (ahora - ultimoCambioBoton >= antirrebote) {
    if (lectura != estadoBoton) {
      estadoBoton = lectura;

      // Guardar la solicitud durante verde o amarillo.
      if (estadoBoton == LOW && (estado == 0 || estado == 1)) {
        solicitudPeaton = true;
      }
    }
  }

  lecturaAnterior = lectura;

  // VERDE: dura 6 segundos.
  if (estado == 0 && ahora - inicioEstado >= 6000UL) {
    digitalWrite(verdeAutos, LOW);
    digitalWrite(amarilloAutos, HIGH);

    estado = 1;
    inicioEstado = ahora;
  }

  // AMARILLO: dura 2 segundos; despues comienza el rojo.
  else if (estado == 1 && ahora - inicioEstado >= 2000UL) {
    digitalWrite(amarilloAutos, LOW);
    digitalWrite(rojoAutos, HIGH);

    // Dar paso al peaton solamente si presiono el boton.
    if (solicitudPeaton) {
      digitalWrite(rojoPeaton, LOW);
      digitalWrite(verdePeaton, HIGH);
    }

    solicitudPeaton = false;
    estado = 2;
    inicioEstado = ahora;
  }

  // ROJO: dura 6 segundos, haya o no cruce peatonal.
  else if (estado == 2 && ahora - inicioEstado >= 6000UL) {
    digitalWrite(verdePeaton, LOW);
    digitalWrite(rojoPeaton, HIGH);

    digitalWrite(rojoAutos, LOW);
    digitalWrite(verdeAutos, HIGH);

    estado = 0;
    inicioEstado = ahora;
  }
}
