#Sandtrix---Game

Esta no es la versión final del juego, pero igualmente está completa. La última versión disponible se subirá a la brevedad e iré actualizando el rendimiento de los códigos cada 
vez que pueda.

Sandtrix es una implementación del clásico concepto de Tetris combinada con
una simulación de arena, desarrollada en lenguaje C utilizando la biblioteca
gráfica SDL2.

#Descripción del juego

Las piezas (tetrominós) caen una por una desde la parte superior del tablero.
Mientras la pieza está en caída, el jugador puede:

-Rotarla.
-Moverla a la izquierda o derecha.
-Acelerar su caída.

Cuando la pieza toca la arena, cada uno de sus píxeles se convierte en un grano
de arena y pasa a formar parte de la simulación.

Las líneas se forman por color: si existe un camino continuo de granos de arena
del mismo color que conecte el borde izquierdo con el derecho, todo ese bloque
es eliminado.

El puntaje se calcula según la cantidad de píxeles eliminados.

#Controles

⬅️ Flecha izquierda: mover pieza a la izquierda.
➡️ Flecha derecha: mover pieza a la derecha.
⬇️ Flecha abajo: acelerar caída.
⬆️ Flecha arriba: rotar pieza.

#Requisitos

-Biblioteca Gráfica SDL2.
-Compilador C (gcc recomendado).
-Sistema Linux(Contar con una terminal Linux es suficiente) / Windows / macOS

#Compilación y ejecución
1) make
2) ./sandtrix

