const int ledVerde = 9;
const int ledAmarillo = 10;
const int ledRojo = 11;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
}

void loop() {
  // LED verde: encendido durante medio segundo
  digitalWrite(ledVerde, HIGH);
  delay(500);
  digitalWrite(ledVerde, LOW);

  // LED amarillo: encendido durante un segundo
  digitalWrite(ledAmarillo, HIGH);
  delay(1000);
  digitalWrite(ledAmarillo, LOW);

  // LED rojo: encendido durante un segundo y medio
  digitalWrite(ledRojo, HIGH);
  delay(1500);
  digitalWrite(ledRojo, LOW);
}