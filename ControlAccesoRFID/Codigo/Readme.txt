Codigo de la practica:

control_acceso_rfid/control_acceso_rfid.ino -> lee el UID de las etiquetas con el RC522 por SPI, lo muestra en el Monitor Serie
                                               y lo compara con el UID autorizado (99 EB 7B 63). LED verde (D7) = acceso permitido,
                                               LED rojo (D6) = acceso denegado. Los LEDs se apagan solos a los 2000 ms usando millis().

Libreria: "MFRC522" (by GithubCommunity), instalada desde el Library Manager del Arduino IDE. SPI.h ya viene incluida.
Monitor Serie a 9600 baudios.

Nota: el sketch incluye la linea "while (!Serial && millis() < 3000)" que necesita el R4 WiFi para no perder los primeros
mensajes del Monitor Serie. Para usar otra etiqueta como autorizada, cambien el arreglo uidAutorizado[] por el UID que
les muestre el Monitor Serie.
