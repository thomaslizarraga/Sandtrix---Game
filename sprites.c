#include "sprite.h"
#include "sprites.h"

#include <stdlib.h>
#include <string.h>

struct sprites{
    /*
    Contenedor es un arreglo que contiene cantidad de sprite_t*
    //si contenedor_sprites==NULL -> cantidad=0;
    */
    sprite_t** contenedor;
    size_t cantidad;
};


sprites_t *sprites_crear(const char *ruta){

    FILE* archivo=fopen(ruta,"rb");
    if(archivo==NULL){
        fprintf(stderr,"No se pudo abrir el archivo");
        return NULL;
    }


    sprites_t* sprites = malloc(sizeof(sprites_t));
    if(sprites==NULL){
        fclose(archivo);
        return NULL;
    }
    
    sprites->contenedor=NULL;
    sprites->cantidad=0;
    sprite_t* sprite;

    while(1){
        sprite=sprite_crear(archivo);
        if(sprite==NULL)
            break;
        
        sprite_t** contenedor_aux=realloc(sprites->contenedor, sizeof(sprite_t*) * (sprites->cantidad + 1));
        if(contenedor_aux==NULL){
            
            break;
        }
        sprites->contenedor=contenedor_aux;
        sprites->contenedor[ sprites->cantidad ] = sprite;
        sprites->cantidad++;
    }
    
    fclose(archivo);
    return sprites;
}

void sprites_destruir(sprites_t *ss){
    for(size_t i=0; i < ss->cantidad; i++ )
        sprite_destruir(ss->contenedor[i]);
    free(ss->contenedor);
    free(ss);
}


size_t sprites_cantidad(const sprites_t *ss){
    return ss->cantidad;
}

sprite_t *sprites_obtener(const sprites_t *ss, const char *etiqueta){
    for(size_t i=0; i< ss->cantidad; i++){
        if( strcmp(sprite_etiqueta(ss->contenedor[i]), etiqueta) == 0 ) {
            return ss->contenedor[i];
        }
    }
    return NULL;
}