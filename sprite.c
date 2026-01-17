
#include <stdint.h>
#include <stdlib.h>

#include "sprite.h"


struct sprite{
    /*
    0 <= ancho <= 255 
    0 <= alto <= 255
    los pixeles se codifican en paquetes de un byte, donde cada byte contiene 8 pixeles. El bit
    menos significativo será el primer pixel mientras que el más significativo será el octavo pixel
    del paquete.
    pixeles es un arreglo de filas(alto) por columas(ancho almacenado en bytes) de pixeles.
    
    etiqueta == 20 char con '\0' al final.
    pixeles != NULL si ancho y alto son válidos.
    
    */
    char* etiqueta;
    uint8_t ancho, alto;
    uint8_t** pixeles;
};


sprite_t *sprite_crear(FILE *fi){

    sprite_t* sprite = malloc( sizeof(sprite_t) );
    if(sprite==NULL){
        return NULL;
    }
    
    
    sprite->etiqueta = malloc( sizeof(char) * 20 );

    if(sprite->etiqueta==NULL){
        free(sprite);
        return NULL;
    }

    if( fread(sprite->etiqueta,sizeof(char),20,fi) != 20 ){
        free(sprite->etiqueta);
        free(sprite);
        return NULL;
    }
    fread(&(sprite->ancho),sizeof(uint8_t),1,fi);
    fread(&(sprite->alto),sizeof(uint8_t),1,fi);
    
    sprite->pixeles=malloc( sizeof(uint8_t*) * (sprite->alto) );
    if( sprite->pixeles==NULL ){
        sprite_destruir(sprite);
        return NULL;
    }
    size_t i;

    for(i=0; i < sprite->alto;  i++){
        (sprite->pixeles[i]) = malloc(  sizeof(uint8_t) * (((sprite->ancho)+7)/8) );
        if( (sprite->pixeles[i]) == NULL){
            sprite->alto = i;
            sprite_destruir(sprite);
            return NULL;
        }
    }
    
    for(i=0; i < (sprite->alto) ; i++)
        fread(sprite->pixeles[i], sizeof(uint8_t), ((sprite->ancho)+7) / 8, fi);
    
    return sprite;
}

void sprite_destruir(sprite_t *s){
    for(size_t i=0; i < s->alto;  i++)
        free(s->pixeles[i]);
    free(s->pixeles);
    free(s->etiqueta);
    free(s);
}


bool sprite_a_pbm(const sprite_t *s, FILE *fo){

    if(s==NULL)
        return false;
    
    fputs("P1\n",fo);
    fprintf(fo,"%zd %zd\n",sprite_ancho(s), sprite_alto(s));
    for(size_t f=0;  f < s->alto;  f++){
        for(size_t c=0; c < s->ancho; c++)
            fprintf(fo, "%d ", sprite_obtener(s, f, c));
        fputc('\n',fo);
    }
    //
    return true;

}

size_t sprite_ancho(const sprite_t *s){
    return (size_t)(s->ancho);
}


size_t sprite_alto(const sprite_t *s){
    return (size_t)(s->alto);
}

char *sprite_etiqueta(const sprite_t *s){
    return s->etiqueta;
}

bool sprite_obtener(const sprite_t *s, size_t fila, size_t col){
    
    if(fila > (s->alto - 1)    ||    col > (8 * s->ancho - 1))
        return false;

    bool valor = ( (s->pixeles[fila][col/8]) >> (col%8) ) & 1; //
    return valor;
}


bool sprite_establecer(sprite_t *s, size_t fila, size_t col, bool valor){

    if(  (fila > (s->alto - 1)) || (col > (8 * s->ancho - 1)) )
        return false;

    uint8_t mask=1;
    if(valor)
        s->pixeles[fila][col/8] = (mask << (col&8) ) | (s->pixeles[fila][col/8]) ;
    else
        s->pixeles[fila][col/8] = (~(mask << (col&8) )) & (s->pixeles[fila][col/8]);
    return true;
    
}
