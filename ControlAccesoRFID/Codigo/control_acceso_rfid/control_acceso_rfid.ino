// Control de acceso con RFID RC522 - Arduino UNO R4 WiFi (versión corregida)
// Librería: "MFRC522" (by GithubCommunity) desde el Library Manager

#include <SPI.h>
#include <MFRC522.h>

// ---------- Pines ----------
#define SS_PIN     10   // SDA del RC522
#define RST_PIN     9   // RST del RC522
#define LED_VERDE   7
#define LED_ROJO    6

// ---------- Configuración ----------
const unsigned long TIEMPO_LED = 2000;   // ms que dura encendido el LED

// UID de la tarjeta AUTORIZADA (cámbienlo por el que les salga en el Monitor Serie)
byte uidAutorizado[] = {0x99, 0xEB, 0x7B, 0x63};
const byte TAM_UID_AUT = sizeof(uidAutorizado);

MFRC522 rfid(SS_PIN, RST_PIN);

// ---------- Variables para millis() ----------
bool ledEncendido = false;
unsigned long tiempoEncendido = 0;
unsigned long ultimaRevision = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 3000);   // El R4 tarda en abrir el puerto USB

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);

  SPI.begin();
  iniciarLector();

  // Verificar comunicación leyendo el registro de versión del chip
  byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print("Version del lector: 0x");
  Serial.println(version, HEX);

  if (version == 0x00 || version == 0xFF) {
    Serial.println("ERROR: No hay comunicacion con el lector RC522.");
    Serial.println("Revisen las conexiones (SPI, 3.3V, GND).");
  } else {
    Serial.println("Comunicacion OK con el lector RC522.");
    Serial.println("Acerquen una tarjeta...");
  }
}

void loop() {
  // 1) Apagar el LED cuando pasen 2000 ms (sin usar delay)
  if (ledEncendido && (millis() - tiempoEncendido >= TIEMPO_LED)) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);
    ledEncendido = false;
  }

  // 2) Cada 5 s revisar que el lector siga respondiendo y con la antena encendida
  if (millis() - ultimaRevision >= 5000) {
    ultimaRevision = millis();
    revisarLector();
  }

  // 3) Revisar si hay una tarjeta nueva (no bloquea)
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial())   return;

  // 4) Mostrar UID en hexadecimal
  mostrarUID();

  // 5) Comparar con el UID autorizado
  if (uidCoincide()) {
    Serial.println("ACCESO PERMITIDO");
    encenderLED(LED_VERDE);
  } else {
    Serial.println("ACCESO DENEGADO");
    encenderLED(LED_ROJO);
  }
  Serial.println();

  // 6) Terminar la comunicación con la tarjeta
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

// Reinicio "duro" del lector + inicialización + antena al máximo
void iniciarLector() {
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, LOW);
  delay(50);
  digitalWrite(RST_PIN, HIGH);
  delay(50);

  rfid.PCD_Init();
  delay(10);
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);   // Máxima ganancia de la antena
  rfid.PCD_AntennaOn();
}

// Si el lector se reinició (p. ej. por un bajón de voltaje), lo vuelve a configurar
void revisarLector() {
  byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  byte ganancia = rfid.PCD_GetAntennaGain();

  if (version == 0x00 || version == 0xFF) {
    Serial.println("AVISO: se perdio la comunicacion con el lector.");
    return;
  }
  if (ganancia != rfid.RxGain_max) {
    Serial.println("AVISO: el lector se reinicio, configurandolo de nuevo...");
    iniciarLector();
  }
}

void mostrarUID() {
  Serial.print("UID:");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(" ");
    if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");  // 0A en vez de A
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
}

bool uidCoincide() {
  if (rfid.uid.size != TAM_UID_AUT) return false;
  for (byte i = 0; i < TAM_UID_AUT; i++) {
    if (rfid.uid.uidByte[i] != uidAutorizado[i]) return false;
  }
  return true;
}

void encenderLED(int pin) {
  // Apaga ambos y enciende solo el indicado; reinicia el conteo de 2 s
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(pin, HIGH);
  tiempoEncendido = millis();
  ledEncendido = true;
}
