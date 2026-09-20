### Funcionamiento

1. Al encender, el servo se coloca en 90° y el Arduino crea la red **Arduino-Servo** (contraseña `12345678`).
2. Conéctate a esa red y abre **http://192.168.4.1** en el navegador.
3. Mueve el control deslizante: el servo sigue el valor mientras arrastras. El botón **Mover Servo** también envía el ángulo.
4. En el monitor serie (9600 baudios) se registra cada ángulo nuevo y el ancho de pulso aplicado.

| Ruta | Acción | Respuesta |
|---|---|---|
| `GET /` | Envía la página de control | HTML |
| `GET /?angle=N` | Mueve el servo y envía la página (funciona sin JavaScript) | HTML |
| `GET /servo?angle=N` | Mueve el servo en tiempo real | Ángulo aplicado, o `400` si no es válido |
| `GET /estado` | Consulta el ángulo actual | Ángulo actual |
| `GET /favicon.ico` | No realiza ninguna acción | `204` |

### Mejoras respecto a la versión inicial

- **Control en tiempo real:** antes había que presionar el botón y la página se recargaba. Ahora el ángulo se envía mientras se arrastra el control, con un solo envío a la vez para no saturar al Arduino.
- **Ángulos duplicados corregidos:** en el monitor serie cada ángulo aparecía dos veces. La causa era que el navegador pide también el ícono (`/favicon.ico`) y en esa petición incluye la dirección anterior (`Referer: ...?angle=50`). Como el programa buscaba `angle=` en toda la petición, volvía a aplicar el ángulo. Ahora solo se lee la primera línea de la petición.
- **Validación:** solo se aceptan números enteros de 0 a 180. Los valores inválidos responden `400` y no mueven el servo.
- El servo solo se escribe cuando el ángulo cambia.
- La página se guarda como texto fijo en memoria y se envía por bloques.
