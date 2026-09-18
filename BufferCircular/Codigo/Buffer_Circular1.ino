#include <Arduino.h>
#include "Arduino_LED_Matrix.h"

// Matriz LED integrada
ArduinoLEDMatrix matriz;

// Pin que genera la interrupción
const byte PIN_PULSADOR = 2;

// Tamaño del buffer circular
const byte TAM_BUFFER = 32;

// Tiempo antirrebote: 40 milisegundos
const unsigned long TIEMPO_REBOTE_US = 40000UL;

// Velocidad de la animación
const unsigned long INTERVALO_ANIMACION_MS = 100;

// Buffer circular de eventos
volatile unsigned long bufferEventos[TAM_BUFFER];

// Índice donde se escribirá el siguiente evento
volatile byte indiceEscritura = 0;

// Índice del siguiente evento que debe leerse
volatile byte indiceLectura = 0;

// Cantidad de eventos pendientes
volatile byte eventosPendientes = 0;

// Datos para el filtro antirrebote
volatile unsigned long ultimoEventoUs = 0;
volatile bool existeUltimoEvento = false;

// Cuenta eventos que llegaron cuando el buffer estaba lleno
volatile unsigned int eventosRechazados = 0;

// Variables de la animación
byte frame[8][12];
byte columna = 0;
unsigned long ultimaAnimacion = 0;

// Contador general de pulsaciones procesadas
unsigned long totalEventos = 0;


// Esta función se ejecuta automáticamente cuando se presiona el botón
void registrarEvento()
{
  unsigned long momentoActual = micros();

  // Filtro antirrebote
  if (existeUltimoEvento &&
      (unsigned long)(momentoActual - ultimoEventoUs) < TIEMPO_REBOTE_US)
  {
    return;
  }

  ultimoEventoUs = momentoActual;
  existeUltimoEvento = true;

  // Si todavía hay espacio, se guarda el evento
  if (eventosPendientes < TAM_BUFFER)
  {
    bufferEventos[indiceEscritura] = momentoActual;

    // Avanza el índice y regresa al inicio cuando llega al final
    indiceEscritura = (indiceEscritura + 1) % TAM_BUFFER;

    eventosPendientes++;
  }
  else
  {
    // El buffer está lleno y no se sobrescribe información anterior
    eventosRechazados++;
  }
}


// Extrae un evento del buffer circular
bool obtenerEvento(unsigned long &momentoEvento)
{
  noInterrupts();

  if (eventosPendientes == 0)
  {
    interrupts();
    return false;
  }

  momentoEvento = bufferEventos[indiceLectura];

  indiceLectura = (indiceLectura + 1) % TAM_BUFFER;

  eventosPendientes--;

  interrupts();

  return true;
}


// Actualiza la animación de la matriz LED
void actualizarAnimacion()
{
  unsigned long ahora = millis();

  if (ahora - ultimaAnimacion >= INTERVALO_ANIMACION_MS)
  {
    ultimaAnimacion = ahora;

    // Apaga todos los LEDs del cuadro
    for (byte fila = 0; fila < 8; fila++)
    {
      for (byte columnaActual = 0; columnaActual < 12; columnaActual++)
      {
        frame[fila][columnaActual] = 0;
      }
    }

    // Dibuja una línea vertical que se mueve
    frame[2][columna] = 1;
    frame[3][columna] = 1;
    frame[4][columna] = 1;
    frame[5][columna] = 1;

    matriz.renderBitmap(frame, 8, 12);

    columna++;

    if (columna >= 12)
    {
      columna = 0;
    }
  }
}


// Informa si se intentaron guardar eventos con el buffer lleno
void revisarEventosRechazados()
{
  static unsigned long ultimoReporte = 0;

  if (millis() - ultimoReporte >= 1000)
  {
    ultimoReporte = millis();

    unsigned int rechazados;

    noInterrupts();
    rechazados = eventosRechazados;
    eventosRechazados = 0;
    interrupts();

    if (rechazados > 0)
    {
      Serial.print("Eventos rechazados por buffer lleno: ");
      Serial.println(rechazados);
    }
  }
}


void setup()
{
  Serial.begin(9600);

  // El botón se conecta entre D2 y GND
  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  // Inicia la matriz LED
  matriz.begin();

  // Configura D2 como entrada de interrupción
  attachInterrupt(
    digitalPinToInterrupt(PIN_PULSADOR),
    registrarEvento,
    FALLING
  );

  Serial.println("Sistema de buffer circular iniciado.");
  Serial.println("Presiona el pulsador para registrar eventos.");
}


void loop()
{
  // La animación continúa constantemente
  actualizarAnimacion();

  // Se procesa solamente un evento por vuelta del loop
  unsigned long momentoEvento;

  if (obtenerEvento(momentoEvento))
  {
    totalEventos++;

    Serial.print("Evento registrado #");
    Serial.print(totalEventos);
    Serial.print(" | Tiempo: ");
    Serial.print(momentoEvento);
    Serial.println(" microsegundos");
  }

  revisarEventosRechazados();
}