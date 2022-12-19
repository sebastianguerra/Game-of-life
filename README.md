
# Game of life

Implementacion del Juego de la vida de John Conway en C para familiarizarme con ncurses y practicar mas el lenguaje.


## Implementacion

El estado del juego se representa en una matriz de WxH, en donde cada celula almacena su estado actual y la cantidad de celulas vecinas vivas a su alrededor.

Para esto usa solo 1 byte. Los primeros 4 bits almacenan la cantidad de celulas vecinas vivas, y el 5to bit almacena el estado actual de la celula.

Las reglas del juego las simplifique a:
- Viva con 2 o 3 vecinos -> viva
- Muerta con 3 vecinos -> viva
- Cualquier otro caso -> muerta

Ya que de esta forma a nivel de bit la unica forma de que se mantenga viva en la siguiente
generacion es que sea:
- 0001001X (18 o 19)
- 00000011 (3)

El primer caso lo simplifico con un desplazamiento a la derecha y comparandolo con un 9. Y el segundo caso lo comparo con el 3.


## TODO

 - [x] Implementar mecanica principal
 - [x] Implementar movimiento con hjkl
 - [x] Facilitar insercion de patrones desde el codigo
 - [ ] Implementar insercion de patrones desde el juego
 - [x] Implementar sistema de detencion y resumen de la simulacion
 - [ ] Mostrar estadisticas en la pantalla
 - [ ] Implementar insertar o eliminar celula viva
 - [x] Implementar modificar velocidad desde el juego