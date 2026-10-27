/*
  Practica 4.1.2  Boton de arranque/paro a prueba de rebotes y apagones
  Placa: Arduino UNO R4 WiFi (matriz LED integrada)

  Un solo boton (D2) arranca y para un motor con puente H.
    - Filtro de rebotes de 25 ms dentro de la clase Boton (sin delay()).
    - Arranque seguro: lo primero en setup() es dejar el motor apagado.
    - Aviso de reinicio al encender o tras una caida de voltaje.
    - Arranque del motor con rampa de PWM (0 a 255 en 1.5 s).
    - Paro inmediato (PWM a 0).
    - La matriz LED muestra el estado del motor.

  Sin delay() en todo el programa.
*/

#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matriz;

// Dibujos de la matriz (8 filas x 12 columnas)
const uint32_t DIBUJO_REINICIO[]   = {0x0F00F00F, 0x00F00F00, 0x000F00F0};   // signo !
const uint32_t DIBUJO_ENCENDIDO[]  = {0x2003C03F, 0x03FC3FC3, 0xF03C0200};   // triangulo (play)
const uint32_t DIBUJO_APAGADO[]    = {0x3FC20420, 0x42042042, 0x042043FC};   // cuadro (stop)

const uint8_t  PIN_BOTON = 2;    // pulsador a GND (INPUT_PULLUP)
const uint8_t  PIN_ENA   = 9;    // L298N/L293D ENA (PWM)
const uint8_t  PIN_IN1   = 7;    // L298N/L293D IN1

const uint32_t VENTANA_MS = 25;  // ventana de estabilidad del filtro
const uint32_t RAMPA_MS   = 1500; // el PWM sube de 0 a 255 en este tiempo
const uint32_t PASO_MS    = 10;   // cada cuanto se actualiza la rampa
const uint32_t AVISO_MS   = 2000; // cuanto dura el aviso de reinicio

class Boton {
  public:
    Boton(uint8_t pin = 2, uint32_t ventana = 25)
      : _pin(pin),
        _ventana(ventana),
        _lecturaAnterior(HIGH),
        _estadoAceptado(HIGH),
        _ultimoCambio(0),
        _huboPulsacion(false) {}

    void iniciar() {
      pinMode(_pin, INPUT_PULLUP);          // suelto = HIGH, oprimido = LOW
    }

    // Se llama en cada vuelta del loop()
    void actualizar() {
      bool lectura = digitalRead(_pin);

      // Cada rebote reinicia el cronometro
      if (lectura != _lecturaAnterior) {
        _ultimoCambio = millis();
      }

      // Si la senal lleva quieta la ventana completa y es distinta
      // al estado que ya teniamos aceptado, se acepta como nuevo estado
      if ((millis() - _ultimoCambio) >= _ventana && lectura != _estadoAceptado) {
        _estadoAceptado = lectura;
        if (_estadoAceptado == LOW) {       // paso de suelto a oprimido
          _huboPulsacion = true;
        }
      }

      _lecturaAnterior = lectura;
    }

    // Entrega cada pulsacion real una sola vez
    bool fuePresionado() {
      if (_huboPulsacion) {
        _huboPulsacion = false;
        return true;
      }
      return false;
    }

  private:
    const uint8_t  _pin;
    const uint32_t _ventana;
    bool     _lecturaAnterior;
    bool     _estadoAceptado;
    uint32_t _ultimoCambio;
    bool     _huboPulsacion;
};

// ---------------------------------------------------

Boton boton(PIN_BOTON, VENTANA_MS);

bool     motorEncendido = false;
bool     enRampa        = false;
uint32_t inicioRampa    = 0;
uint32_t ultimoPaso     = 0;
uint32_t pulsaciones    = 0;

// ---------------------------------------------------

void motorApagar() {                 // paro inmediato
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);
  motorEncendido = false;
  enRampa = false;
}

void motorArrancar() {               // arranque con rampa
  digitalWrite(PIN_IN1, HIGH);
  analogWrite(PIN_ENA, 0);
  motorEncendido = true;
  enRampa = true;
  inicioRampa = millis();
  ultimoPaso  = inicioRampa;
}

void actualizarRampa() {
  if (!enRampa) return;

  uint32_t t = millis() - inicioRampa;
  if (t >= RAMPA_MS) {
    analogWrite(PIN_ENA, 255);
    enRampa = false;
  }
  else if (millis() - ultimoPaso >= PASO_MS) {
    analogWrite(PIN_ENA, (uint8_t)((t * 255UL) / RAMPA_MS));
    ultimoPaso = millis();
  }
}

uint32_t tReinicio = 0;
uint8_t  pantallaActual = 255;       // 0 = aviso, 1 = encendido, 2 = apagado

void mostrar(uint8_t cual) {
  if (cual == pantallaActual) return;   // solo redibuja si cambio
  pantallaActual = cual;
  if      (cual == 0) matriz.loadFrame(DIBUJO_REINICIO);
  else if (cual == 1) matriz.loadFrame(DIBUJO_ENCENDIDO);
  else                matriz.loadFrame(DIBUJO_APAGADO);
}

void actualizarPantalla() {
  if (millis() - tReinicio < AVISO_MS) mostrar(0);       // aviso de reinicio
  else mostrar(motorEncendido ? 1 : 2);                  // estado del motor
}

// ---------------------------------------------------

void setup() {
  // 1) Estado seguro: lo primero es dejar el motor apagado
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);

  boton.iniciar();

  // 2) Aviso de reinicio (pantalla y Monitor Serie)
  matriz.begin();
  tReinicio = millis();
  mostrar(0);
  Serial.begin(9600);
  while (!Serial && millis() < 3000);

  Serial.println(F("*** SISTEMA REINICIADO ***"));
  Serial.println(F("Motor APAGADO. Oprime el boton para arrancar."));
  Serial.println();
}

void loop() {
  boton.actualizar();

  if (boton.fuePresionado()) {
    pulsaciones++;
    Serial.print(F("Pulsacion #"));
    Serial.print(pulsaciones);

    if (motorEncendido) {
      motorApagar();
      Serial.println(F(" -> Motor APAGADO"));
    } else {
      motorArrancar();
      Serial.println(F(" -> Motor ENCENDIDO (rampa)"));
    }
  }

  actualizarRampa();
  actualizarPantalla();
}
