#include <WiFiS3.h>
#include <Servo.h>

Servo servo;

// -------------------------
// CONFIGURACION
// -------------------------
const int PIN_SERVO = 9;

char ssid[] = "Arduino-Servo";
char pass[] = "12345678";

WiFiServer servidor(80);

int angulo = 90;

void setup() {

  Serial.begin(9600);

  // Servo
  servo.attach(PIN_SERVO);
  servo.write(angulo);

  // Crear red WiFi
  Serial.println("Creando red WiFi...");

  if (WiFi.beginAP(ssid, pass) != WL_AP_LISTENING) {
    Serial.println("Error al crear la red WiFi");
    while (true);
  }

  delay(2000);

  servidor.begin();

  Serial.println();
  Serial.println("=================================");
  Serial.println("   CONTROL WEB DEL SERVOMOTOR");
  Serial.println("=================================");

  Serial.print("Red WiFi: ");
  Serial.println(ssid);

  Serial.print("Contrasena: ");
  Serial.println(pass);

  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  WiFiClient cliente = servidor.available();

  if (cliente) {

    String peticion = "";

    while (cliente.connected()) {

      if (cliente.available()) {

        char c = cliente.read();
        peticion += c;

        // Terminó la petición HTTP
        if (peticion.endsWith("\r\n\r\n")) {

          // Buscar angle=
          int posicion = peticion.indexOf("angle=");

          if (posicion >= 0) {

            int inicio = posicion + 6;
            int fin = peticion.indexOf(' ', inicio);

            if (fin == -1) {
              fin = peticion.length();
            }

            String dato = peticion.substring(inicio, fin);

            int ampersand = dato.indexOf('&');

            if (ampersand >= 0) {
              dato = dato.substring(0, ampersand);
            }

            int nuevoAngulo = dato.toInt();

            if (nuevoAngulo >= 0 && nuevoAngulo <= 180) {

              angulo = nuevoAngulo;
              servo.write(angulo);

              Serial.print("Nuevo angulo: ");
              Serial.println(angulo);
            }
          }

          // Respuesta HTTP
          cliente.println("HTTP/1.1 200 OK");
          cliente.println("Content-Type: text/html");
          cliente.println("Connection: close");
          cliente.println();

          // Página web
          cliente.println("<!DOCTYPE html>");
          cliente.println("<html>");
          cliente.println("<head>");

          cliente.println(
            "<meta name='viewport' "
            "content='width=device-width, initial-scale=1'>"
          );

          cliente.println("<title>Control Servo</title>");

          cliente.println("<style>");
          cliente.println("body {");
          cliente.println("font-family: Arial;");
          cliente.println("text-align: center;");
          cliente.println("background: #f2f2f2;");
          cliente.println("margin-top: 50px;");
          cliente.println("}");

          cliente.println(".caja {");
          cliente.println("background: white;");
          cliente.println("padding: 30px;");
          cliente.println("margin: auto;");
          cliente.println("max-width: 400px;");
          cliente.println("border-radius: 15px;");
          cliente.println("}");

          cliente.println("input[type=range] {");
          cliente.println("width: 100%;");
          cliente.println("}");

          cliente.println("button {");
          cliente.println("padding: 12px 25px;");
          cliente.println("font-size: 18px;");
          cliente.println("margin-top: 20px;");
          cliente.println("}");

          cliente.println("</style>");

          cliente.println("</head>");

          cliente.println("<body>");

          cliente.println("<div class='caja'>");

          cliente.println("<h1>Control del Servomotor</h1>");

          cliente.print("<h2>Angulo actual: ");
          cliente.print(angulo);
          cliente.println("&deg;</h2>");

          cliente.println("<form action='/' method='GET'>");

          cliente.print(
            "<input type='range' "
            "name='angle' "
            "min='0' "
            "max='180' "
            "value='"
          );

          cliente.print(angulo);

          cliente.println(
            "' oninput='valor.innerText=this.value'>"
          );

          cliente.println("<h2>");
          cliente.println("<span id='valor'>");

          cliente.print(angulo);

          cliente.println("</span>&deg;</h2>");

          cliente.println(
            "<button type='submit'>Mover Servo</button>"
          );

          cliente.println("</form>");

          cliente.println("</div>");

          cliente.println("</body>");
          cliente.println("</html>");

          break;
        }
      }
    }

    delay(1);
    cliente.stop();
  }
}