
#ifndef IMAGEN_H
#define IMAGEN_H
#include "color.h"
#include "sprite.h"
#include "sprites.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct imagen imagen_t;


//EJ1 y hay que adaptarlas

//inicialice en 0 cada una de las posiciones de la matriz recibida.
void inicializar_matriz(size_t filas, size_t columnas, char matriz[filas][columnas]);

//que realice una copia de la matriz origen en la matriz destino.
void copiar_matriz(size_t filas, size_t columnas, char destino[filas][columnas], char origen[filas][columnas]); 

/*imprime por stdout una imagen en formato PBM en base a la matriz. Las celdas que estén en 0 se imprimirán como 0,
mientras que las celdas que estén en otro valor se imprimirán como 1. Es decir, tendremos una imagen blanco y 
negro donde las celdas en cero serán blancas y las de cualquier otro valor serán negras.*/
void imprimir_ppm(size_t filas, size_t columnas, char matriz[filas][columnas]);

//que copie los elementos de la submatriz en la matriz en una determinada posicion dentro de la matriz
bool pegar_submatriz(size_t mfs, size_t mcs, char matriz[mfs][mcs], size_t sfs, size_t scs, char submatriz[sfs][scs], size_t fi, size_t ci);



//que dada la celula matriz sume el valor de las 8 células que la rodean sin considerar bordes.
int sumar_entorno(size_t filas, size_t columnas, char matriz[filas][columnas], size_t fc, size_t cc);

//esta es para la simulacion de arena
//que evolucione UNA VEZ el estado inicial de un autómata para generar el estado final.

void evolucionar_celulas(size_t filas, size_t columnas, char final[filas][columnas], char inicial[filas][columnas]);


//EJ2
//crea una imagen de ancho x alto de pixeles color_t, sin inicializar el valor de sus píxeles. La función debe 
//devolver NULL en caso de falla.
imagen_t *imagen_crear(size_t ancho, size_t alto);

//libera la memoria
void imagen_destruir(imagen_t *imag);

//clonar imagen en nueva memoria
imagen_t *copiar_imagen(const imagen_t *original);

//imprime por stdout la imagen i en formato PPM en RGB de 24 bits.
void imagen_escribir_ppm(const imagen_t *i);

//lee de stdin (en nuestro tp no será de stdin asumo)
//una imagen en formato PPM RGB de 24 bits y la cargue en memoria como DEF

imagen_t *imagen_leer_ppm(FILE* a); 

//NUEVAS

bool pegar_subimagen_tranparencia(imagen_t* imagen, imagen_t* subimagen, int posicion_f, int posicion_c);

size_t imagen_alto(imagen_t* i);

size_t imagen_ancho(imagen_t* i);

//esta va a servir para rotar la pieza
imagen_t *imagen_rotar_antihorario(const imagen_t *original);

void imagen_establecer_pixel(imagen_t *imag, size_t fil, size_t col,color_t c);

//devuelve un color de un determinado pixel de la imagen
// 0 <= fil <= imagen_alto - 1        0 <= col <= imagen_ancho - 1      
color_t imagen_obtener_pixel(const imagen_t *imag, int fil, int col);

imagen_t* crear_tablero_negro();


#endif
