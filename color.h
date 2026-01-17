#ifndef COLOR_H
#define COLOR_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef uint16_t color_t;

//setters
//con esta funcion me genero un color a partir a las componentes D E F
color_t color_desde_def(uint8_t d, uint8_t e, uint8_t f);


//getters
//para obtener gama, saturacion y luminosidad 
uint8_t color_d(color_t c);
uint8_t color_e(color_t c);
uint8_t color_f(color_t c);


//crea un DEF a partir de un RGB
color_t color_desde_rgb(uint8_t r, uint8_t g, uint8_t b);

//convierte un color_t (DEF) a sus componentes equivalentes r, g y b de 24 bits y las devuelva.
void color_a_rgb(color_t c, uint8_t *r, uint8_t *g, uint8_t *b);


//NUEVAS

//establece el color negro
color_t color_negro(void);
//chequeo si el color es negro
bool color_es_negro(color_t c);

//chequeo si dos colores tienen la misma gama, o sea, si son del mismo color
bool mismo_color(color_t color1, color_t color2);

char dar_color();



#endif
