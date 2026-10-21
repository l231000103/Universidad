Codigo de la practica:

Practica_4_1_1_A/Practica_4_1_1_A.ino -> define la clase Relevador (pin, activo en alto o en bajo, estado, momento del ultimo cambio
                                         y tiempo minimo de 1000 ms entre cambios) y crea dos objetos: el motor con L298N (D7,
                                         activo en alto) y el LED al reves (D6, activo en bajo). Se controla desde el Monitor Serie
                                         con los comandos M1, M0, L1, L0 y E. Sin delay(); el programa principal nunca llama digitalWrite().

Librerias: no necesita ninguna, solo el paquete de la placa Arduino UNO R4 Boards.

IMPORTANTE: el Monitor Serie debe estar a 9600 baudios y con la opcion "Nueva linea"; si no, los comandos no se reciben completos.

Ajustes en el codigo:
- Relevador motor(7, false): pin y tipo de logica del motor (false = activo en alto).
- Relevador led(6, true): pin y tipo de logica del LED (true = activo en bajo).
- tMinimo (tercer parametro del constructor, 1000 ms por defecto): tiempo minimo entre cambios.
