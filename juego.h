#ifndef JUEGO_H
#define JUEGO_H

#include "imagen.h"

//una vez detectada en donde se generó una linea comienza a expandirse para eliminar todo el bloque
void expandir(color_t color,int fila, int columna,imagen_t* tablero, imagen_t* borrar, size_t alto_tablero, 
    size_t ancho_tablero);

bool pieza_no_existe(imagen_t* t);

void remover_linea(imagen_t* tablero, imagen_t* borrar, size_t alto, size_t ancho, unsigned int* contador_puntaje);

//pregunta si llegue a lo mas alto del tablero
bool toque_alto(imagen_t* tablero);

bool pieza_toca_arena(imagen_t* pieza, imagen_t* tablero, size_t fp, size_t cp);

void simulacion_arena (imagen_t *tablero);

bool se_genero_linea(size_t alto_tablero, size_t ancho_tablero, imagen_t* tablero, imagen_t* visitados,int fila, int columna, color_t color);

//genera miniatura de la pieza siguiente
imagen_t* generar_miniatura(sprite_t* sprite);

imagen_t* generar_numeros(char* aux, sprites_t* sprites);

imagen_t* generar_tiempo(unsigned int ticks, sprites_t* sprites);

void generar_puntaje(unsigned int contador_puntaje, imagen_t** puntos, unsigned int contador_clears, imagen_t** clears, sprites_t* sprites);

imagen_t* pieza_crear(color_t color, sprite_t* sprite);

imagen_t* generar_tubo(sprites_t* sprites, color_t color);

imagen_t* crear_tablero_negro();

void desplazamientos(char direccion, size_t* cp, char* desplazamiento_restante);

void aumento_de_velocidad(unsigned int ticks, unsigned int* ticks_previo, unsigned int* tiempo_aceleracion,
    float* suma_1_segundos ,float* velocidad, float* f_, 
    bool* aceleracion_activada);

char* dar_pieza_random();

void puntaje_en_pantalla(unsigned int contador_puntaje, unsigned int contador_clears);

#endif