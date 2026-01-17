#ifndef SPRITES_H
#define SPRITES_H

#include "sprite.h"

typedef struct sprites sprites_t;

//constructor 

//dada la ruta de un archivo binario devuelva un TDA sprites que contenga TODOS los sprites del archivo
sprites_t* sprites_crear(const char *ruta);


//destructor

/*Que libere toda la memoria asociada al TDA*/
void sprites_destruir(sprites_t *ss);


//getters

//devuelve la cantidad de sprites que hay dentro del TDA.
size_t sprites_cantidad(const sprites_t *ss);


//dada el nombre de un sprite devuelva el sprite correspondiente del contenedor.
//Si no hubiera un sprite con esa etiqueta se devolverá NULL
sprite_t *sprites_obtener(const sprites_t *ss, const char *etiqueta);


#endif

