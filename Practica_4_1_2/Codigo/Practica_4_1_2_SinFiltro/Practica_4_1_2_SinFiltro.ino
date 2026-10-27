/*
  Practica 4.1.2  Version SIN FILTRO (paso 2 del desarrollo)

  Lee el boton directamente en cada vuelta del loop() y cuenta cada
  paso de suelto (HIGH) a oprimido (LOW). Cada pulsacion detectada
  cambia el motor. Sirve para comparar contra la version con filtro.

  Sin delay().
*/

const uint8_t PIN_BOTON = 2;
const uint8_t PIN_ENA   = 9;
const uint8_t PIN_IN1   = 7;

bool     lecturaAnterior = HIGH;
bool     motorEncendido  = false;
uint32_t contador        = 0;

void setup() {
  // Estado seguro
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);

  pinMode(PIN_BOTON, INPUT_PULLUP);

  Serial.begin(9600);
  while (!Serial && millis() < 3000);
  Serial.println(F("Version SIN FILTRO listo. Motor APAGADO."));
}

void loop() {
  bool lectura = digitalRead(PIN_BOTON);

  // Solo cuenta al pasar de suelto a oprimido (soltar no cuenta)
  if (lecturaAnterior == HIGH && lectura == LOW) {
    contador++;
    motorEncendido = !motorEncendido;

    if (motorEncendido) {
      digitalWrite(PIN_IN1, HIGH);
      analogWrite(PIN_ENA, 255);        // de golpe, sin rampa
    } else {
      analogWrite(PIN_ENA, 0);
      digitalWrite(PIN_IN1, LOW);
    }

    Serial.print(F("Pulsaciones contadas: "));
    Serial.print(contador);
    Serial.println(motorEncendido ? F("  Motor ENCENDIDO") : F("  Motor APAGADO"));
  }

  lecturaAnterior = lectura;
}
