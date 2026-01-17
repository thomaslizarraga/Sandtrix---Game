#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct sprite sprite_t;

//constructor - destructor

//sea fi un archivo abierto en modo lectura binaria, lea del archivo un (y sólo un) sprite y lo devuelva.
//En caso de falla o de que el archivo se terminara debe devolverse NULL 
sprite_t *sprite_crear(FILE *fi);

//libera memoria
void sprite_destruir(sprite_t *s);

//primitivas

//dado un archivo fo abierto en modo 
//escritura de texto genere un PBM (blanco y negro, EJ1) con el contenido del sprite en dicho archivo.
bool sprite_a_pbm(const sprite_t *s, FILE *fo);

//getters

//devuele el ancho del sprite en cantidad de columnas
size_t sprite_ancho(const sprite_t *s);

//devuelve el alto del sprite en cantidad de filas
size_t sprite_alto(const sprite_t *s);

//devuelvo el nombre del sprite
char *sprite_etiqueta(const sprite_t *s);

//devuelva el valor del pixel de la coordenada (fila, col) del sprite s.
bool sprite_obtener(const sprite_t *s, size_t fila, size_t col);

//setter
//escribe el valor dado en el pixel de coordenada (fila, col) del sprite s.
bool sprite_establecer(sprite_t *s, size_t fila, size_t col, bool valor);

#endif