/*
 * ==========================================================================
 *  Control de servomotor por interfaz web
 * --------------------------------------------------------------------------
 *  Placa   : Arduino UNO R4 WiFi
 *  Actuador: Servomotor MG996R (señal en el pin D9)
 *
 *  El Arduino crea su propia red WiFi (modo punto de acceso) y funciona como
 *  servidor web. Desde el navegador del celular o la computadora se abre la
 *  página http://192.168.4.1 y, al mover el control deslizante, el ángulo
 *  se envía al Arduino en tiempo real y el servo se posiciona en ese ángulo.
 *
 *  Rutas del servidor:
 *    GET /                 -> página de control
 *    GET /?angle=N         -> mueve el servo a N grados y devuelve la página
 *                             (funciona aunque el navegador no tenga JavaScript)
 *    GET /servo?angle=N    -> mueve el servo a N grados (lo usa la página
 *                             mientras se arrastra el control); responde el
 *                             ángulo aplicado en texto plano
 *    GET /estado           -> responde el ángulo actual en texto plano
 *
 *  Conexiones (ver carpeta Diagrama):
 *    Cable de señal del servo (naranja) -> D9
 *    Cable rojo del servo (V+)          -> + de la fuente externa (4.8 a 6 V)
 *    Cable café del servo (GND)         -> - de la fuente externa
 *    GND del Arduino                    -> - de la fuente externa (tierra común)
 *    El 5V del Arduino NO se conecta al servo.
 * ==========================================================================
 */

#include <WiFiS3.h>
#include <Servo.h>

// ---------------------------------------------------------------------------
// CONFIGURACIÓN
// ---------------------------------------------------------------------------
const int PIN_SERVO      = 9;     // pin PWM de la señal del servo
const int ANGULO_MIN     = 0;     // límite inferior permitido
const int ANGULO_MAX     = 180;   // límite superior permitido
const int ANGULO_INICIAL = 90;    // posición al encender

// Ancho de pulso para 0° y 180° (valores por defecto de la librería Servo)
const int PULSO_MIN_US = 544;
const int PULSO_MAX_US = 2400;

// Red WiFi que crea el Arduino (la contraseña debe tener 8 caracteres o más)
const char NOMBRE_RED[] = "Arduino-Servo";
const char CLAVE_RED[]  = "12345678";

const unsigned long TIEMPO_ESPERA_MS = 1000;  // tiempo máximo para leer una petición
const size_t        TAM_PETICION     = 128;   // largo máximo de la primera línea HTTP

// ---------------------------------------------------------------------------
// OBJETOS Y VARIABLES GLOBALES
// ---------------------------------------------------------------------------
WiFiServer servidor(80);
Servo servo;

int anguloActual = ANGULO_INICIAL;
int estadoWiFi   = WL_IDLE_STATUS;

// ---------------------------------------------------------------------------
// PÁGINA WEB
// Se guarda como texto fijo en la memoria flash. La página pide el ángulo
// actual al cargar (/estado) y envía el nuevo ángulo mientras se mueve el
// control deslizante (/servo?angle=N), sin recargar la página.
// ---------------------------------------------------------------------------
const char PAGINA_HTML[] = R"HTML(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Control Servo</title>
<style>
body{font-family:Arial,sans-serif;text-align:center;background:#f2f2f2;margin:0;padding-top:50px}
.caja{background:#fff;padding:30px;margin:auto;max-width:400px;border-radius:15px}
input[type=range]{width:100%}
button{padding:12px 25px;font-size:18px;margin-top:20px}
#estado{color:#666;font-size:14px;margin-top:14px}
</style>
</head>
<body>
<div class="caja">
<h1>Control del Servomotor</h1>
<h2>&Aacute;ngulo actual: <span id="actual">--</span>&deg;</h2>
<form action="/" method="GET">
<input type="range" id="control" name="angle" min="0" max="180" value="90">
<h2><span id="valor">90</span>&deg;</h2>
<button type="submit">Mover Servo</button>
</form>
<p id="estado">Conectando...</p>
</div>
<script>
const control = document.getElementById('control');
const valor   = document.getElementById('valor');
const actual  = document.getElementById('actual');
const estado  = document.getElementById('estado');
let ocupado = false;
let pendiente = null;

// Envía el ángulo al Arduino. Si ya hay un envío en curso, guarda solo el
// último valor para no saturar al servidor mientras se arrastra el control.
function enviar(angulo) {
  if (ocupado) { pendiente = angulo; return; }
  ocupado = true;
  fetch('/servo?angle=' + angulo)
    .then(r => r.ok ? r.text() : Promise.reject(r.status))
    .then(t => { actual.textContent = t.trim(); estado.textContent = 'Conectado: tiempo real'; })
    .catch(() => { estado.textContent = 'Sin respuesta del Arduino'; })
    .finally(() => {
      ocupado = false;
      if (pendiente !== null) { const a = pendiente; pendiente = null; enviar(a); }
    });
}

// Mientras se arrastra el control, el servo sigue al valor
control.addEventListener('input', () => {
  valor.textContent = control.value;
  enviar(control.value);
});

// Con JavaScript activo el botón también envía sin recargar la página
document.querySelector('form').addEventListener('submit', e => {
  e.preventDefault();
  enviar(control.value);
});

// Al abrir la página se muestra la posición real del servo
fetch('/estado')
  .then(r => r.text())
  .then(t => {
    const a = parseInt(t, 10);
    if (!isNaN(a)) { control.value = a; valor.textContent = a; actual.textContent = a; }
    estado.textContent = 'Conectado: tiempo real';
  })
  .catch(() => { estado.textContent = 'Sin respuesta del Arduino'; });
</script>
</body>
</html>
)HTML";

// ---------------------------------------------------------------------------
// FUNCIONES DEL SERVO
// ---------------------------------------------------------------------------

// Mueve el servo solo si el ángulo es distinto al actual
void moverServo(int angulo) {
  if (angulo == anguloActual) {
    return;
  }
  anguloActual = angulo;
  servo.write(anguloActual);

  Serial.print("Nuevo angulo: ");
  Serial.print(anguloActual);
  Serial.print(" grados | pulso: ");
  Serial.print(servo.readMicroseconds());
  Serial.println(" us");
}

// Busca "angle=" SOLO dentro de la ruta de la petición (primera línea).
// Devuelve true y guarda el valor si el número es válido (0 a 180).
bool leerAngulo(const char* ruta, int& angulo) {
  const char* consulta = strchr(ruta, '?');
  if (consulta == nullptr) {
    return false;
  }

  const char* p = strstr(consulta, "angle=");
  if (p == nullptr) {
    return false;
  }
  // Evita confundir parámetros como "xangle=" con "angle="
  if (p[-1] != '?' && p[-1] != '&') {
    return false;
  }
  p += 6;  // salta "angle="

  int valor = 0;
  int digitos = 0;
  while (*p >= '0' && *p <= '9') {
    valor = valor * 10 + (*p - '0');
    digitos++;
    p++;
    if (digitos > 3) {
      return false;  // demasiados dígitos
    }
  }

  // Debe haber al menos un dígito y el número debe terminar ahí
  if (digitos == 0 || (*p != '\0' && *p != '&')) {
    return false;
  }
  if (valor < ANGULO_MIN || valor > ANGULO_MAX) {
    return false;
  }

  angulo = valor;
  return true;
}

// ---------------------------------------------------------------------------
// FUNCIONES DEL SERVIDOR WEB
// ---------------------------------------------------------------------------

// Lee la primera línea de la petición ("GET /ruta HTTP/1.1") y descarta los
// encabezados. La versión anterior buscaba "angle=" en toda la petición, y el
// encabezado Referer de la petición del ícono (favicon) hacía que cada ángulo
// se procesara dos veces.
bool leerPeticion(WiFiClient& cliente, char* linea, size_t tam) {
  size_t n = 0;
  bool lineaCompleta = false;
  bool lineaVacia = true;
  unsigned long inicio = millis();

  while (cliente.connected() && millis() - inicio < TIEMPO_ESPERA_MS) {
    if (!cliente.available()) {
      delay(1);
      continue;
    }
    char c = cliente.read();

    if (!lineaCompleta) {
      if (c == '\n') {
        lineaCompleta = true;
      } else if (c != '\r' && n < tam - 1) {
        linea[n++] = c;
      }
      continue;
    }

    // Descarta los encabezados hasta encontrar la línea vacía final
    if (c == '\n') {
      if (lineaVacia) {
        break;
      }
      lineaVacia = true;
    } else if (c != '\r') {
      lineaVacia = false;
    }
  }

  linea[n] = '\0';
  return lineaCompleta && n > 0;
}

// Envía el encabezado HTTP de la respuesta
void enviarEncabezado(WiFiClient& cliente, const char* estado, const char* tipo) {
  cliente.print("HTTP/1.1 ");
  cliente.println(estado);
  cliente.print("Content-Type: ");
  cliente.println(tipo);
  cliente.println("Cache-Control: no-store");
  cliente.println("Connection: close");
  cliente.println();
}

// Envía un texto corto (respuestas de /servo y /estado)
void enviarTexto(WiFiClient& cliente, const char* estado, const String& texto) {
  enviarEncabezado(cliente, estado, "text/plain; charset=utf-8");
  cliente.print(texto);
}

// Envía la página en bloques para no saturar el módulo WiFi
void enviarPagina(WiFiClient& cliente) {
  enviarEncabezado(cliente, "200 OK", "text/html; charset=utf-8");
  const size_t BLOQUE = 512;
  size_t total = strlen(PAGINA_HTML);
  for (size_t i = 0; i < total; i += BLOQUE) {
    size_t n = (total - i < BLOQUE) ? (total - i) : BLOQUE;
    cliente.write((const uint8_t*)(PAGINA_HTML + i), n);
  }
}

// Decide qué hacer con cada petición según su ruta
void atenderCliente(WiFiClient& cliente) {
  char linea[TAM_PETICION];
  if (!leerPeticion(cliente, linea, sizeof(linea))) {
    return;
  }

  // Separa método y ruta: "GET /servo?angle=90 HTTP/1.1"
  if (strncmp(linea, "GET ", 4) != 0) {
    enviarTexto(cliente, "405 Method Not Allowed", "Solo se acepta GET");
    return;
  }
  char* ruta = linea + 4;
  char* espacio = strchr(ruta, ' ');
  if (espacio != nullptr) {
    *espacio = '\0';
  }

  int angulo;

  if (strncmp(ruta, "/servo", 6) == 0 && (ruta[6] == '?' || ruta[6] == '\0')) {
    // Petición en tiempo real desde el control deslizante
    if (leerAngulo(ruta, angulo)) {
      moverServo(angulo);
      enviarTexto(cliente, "200 OK", String(anguloActual));
    } else {
      enviarTexto(cliente, "400 Bad Request", "Angulo invalido (0 a 180)");
    }
  }
  else if (strcmp(ruta, "/estado") == 0) {
    enviarTexto(cliente, "200 OK", String(anguloActual));
  }
  else if (strcmp(ruta, "/favicon.ico") == 0) {
    // El navegador pide un ícono; se responde sin contenido
    enviarEncabezado(cliente, "204 No Content", "text/plain");
  }
  else if (ruta[0] == '/' && (ruta[1] == '\0' || ruta[1] == '?')) {
    // Página principal (también acepta /?angle=N del formulario)
    if (leerAngulo(ruta, angulo)) {
      moverServo(angulo);
    }
    enviarPagina(cliente);
  }
  else {
    enviarTexto(cliente, "404 Not Found", "Ruta no encontrada");
  }
}

// Avisa por el monitor serie cuando un dispositivo se conecta a la red
void revisarConexiones() {
  int estado = WiFi.status();
  if (estado == estadoWiFi) {
    return;
  }
  estadoWiFi = estado;
  if (estado == WL_AP_CONNECTED) {
    Serial.println("Dispositivo conectado a la red del Arduino");
  } else if (estado == WL_AP_LISTENING) {
    Serial.println("Esperando dispositivos...");
  }
}

// ---------------------------------------------------------------------------
// CONFIGURACIÓN INICIAL
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 3000) {
    // espera breve al monitor serie (no se bloquea si no está abierto)
  }

  // Servo en posición inicial
  servo.attach(PIN_SERVO, PULSO_MIN_US, PULSO_MAX_US);
  servo.write(anguloActual);

  // Verifica el módulo WiFi
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("ERROR: no se encontro el modulo WiFi");
    while (true) {
      delay(1000);
    }
  }

  // Crea la red WiFi
  Serial.println("Creando red WiFi...");
  estadoWiFi = WiFi.beginAP(NOMBRE_RED, CLAVE_RED);
  if (estadoWiFi != WL_AP_LISTENING) {
    Serial.println("ERROR: no se pudo crear la red WiFi");
    while (true) {
      delay(1000);
    }
  }
  delay(2000);  // tiempo para que la red quede lista

  servidor.begin();

  Serial.println();
  Serial.println("=================================");
  Serial.println("   CONTROL WEB DEL SERVOMOTOR");
  Serial.println("=================================");
  Serial.print("Red WiFi: ");
  Serial.println(NOMBRE_RED);
  Serial.print("Contrasena: ");
  Serial.println(CLAVE_RED);
  Serial.print("Abrir en el navegador: http://");
  Serial.println(WiFi.localIP());
  Serial.print("Angulo inicial: ");
  Serial.println(anguloActual);
}

// ---------------------------------------------------------------------------
// PROGRAMA PRINCIPAL
// ---------------------------------------------------------------------------
void loop() {
  revisarConexiones();

  WiFiClient cliente = servidor.available();
  if (cliente) {
    atenderCliente(cliente);
    delay(1);
    cliente.stop();
  }
}
