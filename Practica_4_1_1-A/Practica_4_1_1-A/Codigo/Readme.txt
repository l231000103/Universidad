Codigo de la practica:

Practica_4_1_1_A/Practica_4_1_1_A.ino -> sketch para Arduino UNO R4 WiFi. Define la clase Relevador (pin, activo en alto o en bajo, estado, momento del ultimo cambio y tiempo minimo de 1000 ms entre cambios) y crea dos objetos: el motor con L298N (D7, activo en alto) y el LED al reves (D6, activo en bajo). Se controla desde el Monitor serie a 9600 baudios con los comandos M1, M0, L1, L0 y E. No usa delay() y el programa principal nunca llama digitalWrite().

Nota: en el Monitor serie hay que seleccionar "Nueva linea" para que los comandos se reciban completos.
