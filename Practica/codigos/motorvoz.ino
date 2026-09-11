#include <WiFiS3.h>
#include <Arduino_LED_Matrix.h>

// ==================================================
// DATOS DEL WI-FI
// ==================================================

const char* SSID = "NOMBRE_DE_TU_RED";
const char* PASS = "CONTRASENA_DE_TU_RED";

// ==================================================
// CONEXIONES
// ==================================================

// LED externo
const int LED_EXTERNO = 4;

// Puente H L298N
const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;

// ==================================================
// VELOCIDADES
// ==================================================

const int PWM[4] = {
  0,    // Detenido
  110,  // Velocidad baja
  180,  // Velocidad media
  255   // Velocidad máxima
};

int nivel = 0;

// 0 = detenido
// 1 = avanzar
// -1 = retroceder
int direccion = 0;

// ==================================================
// WI-FI Y MATRIZ
// ==================================================

WiFiServer servidor(80);
ArduinoLEDMatrix matriz;

// Todos los LED de la matriz encendidos
uint8_t dibujo[8][12] = {
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1}
};

// ==================================================
// CONTROL DEL MOTOR
// ==================================================

void aplicarVelocidad() {
  if (direccion == 0 || nivel == 0) {
    // Detener
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
  else if (direccion == 1) {
    // Avanzar
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, PWM[nivel]);
  }
  else if (direccion == -1) {
    // Retroceder
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, PWM[nivel]);
  }

  Serial.print("Direccion: ");

  if (direccion == 1) {
    Serial.print("avanzando");
  }
  else if (direccion == -1) {
    Serial.print("retrocediendo");
  }
  else {
    Serial.print("detenido");
  }

  Serial.print(" | Nivel: ");
  Serial.print(nivel);
  Serial.print(" | PWM: ");
  Serial.println(PWM[nivel]);
}

// ==================================================
// CONFIGURACIÓN
// ==================================================

void setup() {
  Serial.begin(9600);

  while (!Serial && millis() < 3000) {
  }

  pinMode(LED_EXTERNO, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(LED_EXTERNO, LOW);

  direccion = 0;
  nivel = 0;
  aplicarVelocidad();

  // Iniciar y apagar la matriz
  matriz.begin();
  matriz.clear();

  // Revisar el módulo Wi-Fi
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("ERROR: no se encontro el modulo Wi-Fi.");

    while (true) {
      delay(1000);
    }
  }

  Serial.print("Conectando al Wi-Fi");

  WiFi.begin(SSID, PASS);

  unsigned long inicio = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - inicio < 30000) {

    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("No se pudo conectar al Wi-Fi.");
    Serial.println("Revisa el nombre y la contrasena.");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("Arduino conectado correctamente.");
  Serial.print("IP del Arduino: http://");
  Serial.println(WiFi.localIP());

  servidor.begin();

  Serial.println("Servidor iniciado.");
}

// ==================================================
// PROGRAMA PRINCIPAL
// ==================================================

void loop() {
  WiFiClient cliente = servidor.available();

  if (!cliente) {
    return;
  }

  cliente.setTimeout(1000);

  String peticion = cliente.readStringUntil('\r');

  Serial.println();
  Serial.print("Peticion recibida: ");
  Serial.println(peticion);

  String respuesta = "Orden no reconocida";

  // ------------------------------------------------
  // LED EXTERNO
  // ------------------------------------------------

  if (peticion.indexOf("/led/on") >= 0) {
    digitalWrite(LED_EXTERNO, HIGH);
    respuesta = "LED encendido";
  }
  else if (peticion.indexOf("/led/off") >= 0) {
    digitalWrite(LED_EXTERNO, LOW);
    respuesta = "LED apagado";
  }

  // ------------------------------------------------
  // MATRIZ LED
  // ------------------------------------------------

  else if (peticion.indexOf("/matriz/on") >= 0) {
    matriz.renderBitmap(dibujo, 8, 12);
    respuesta = "Matriz encendida";
  }
  else if (peticion.indexOf("/matriz/off") >= 0) {
    matriz.clear();
    respuesta = "Matriz apagada";
  }

  // ------------------------------------------------
  // AVANZAR
  // ------------------------------------------------

  else if (peticion.indexOf("/motor/avanza/baja") >= 0) {
    direccion = 1;
    nivel = 1;

    aplicarVelocidad();

    respuesta = "Avanzando velocidad baja";
  }
  else if (peticion.indexOf("/motor/avanza/media") >= 0) {
    direccion = 1;
    nivel = 2;

    aplicarVelocidad();

    respuesta = "Avanzando velocidad media";
  }
  else if (peticion.indexOf("/motor/avanza/maxima") >= 0) {
    direccion = 1;
    nivel = 3;

    aplicarVelocidad();

    respuesta = "Avanzando velocidad maxima";
  }

  // ------------------------------------------------
  // RETROCEDER
  // ------------------------------------------------

  else if (peticion.indexOf("/motor/retrocede/baja") >= 0) {
    direccion = -1;
    nivel = 1;

    aplicarVelocidad();

    respuesta = "Retrocediendo velocidad baja";
  }
  else if (peticion.indexOf("/motor/retrocede/media") >= 0) {
    direccion = -1;
    nivel = 2;

    aplicarVelocidad();

    respuesta = "Retrocediendo velocidad media";
  }
  else if (peticion.indexOf("/motor/retrocede/maxima") >= 0) {
    direccion = -1;
    nivel = 3;

    aplicarVelocidad();

    respuesta = "Retrocediendo velocidad maxima";
  }

  // ------------------------------------------------
  // DETENER MOTOR
  // ------------------------------------------------

  else if (peticion.indexOf("/motor/detener") >= 0) {
    direccion = 0;
    nivel = 0;

    aplicarVelocidad();

    respuesta = "Motor detenido";
  }

  // ------------------------------------------------
  // RESPUESTA PARA APP INVENTOR
  // ------------------------------------------------

  cliente.println("HTTP/1.1 200 OK");
  cliente.println("Content-Type: text/plain; charset=utf-8");
  cliente.println("Connection: close");
  cliente.println();
  cliente.println(respuesta);

  delay(5);
  cliente.stop();
}
