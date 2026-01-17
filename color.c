#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "color.h"
#include <stdlib.h>

#define MASK_F 0x1f
#define MASK_E 0x3f

#define N_16 2<<3
#define N_32 2<<4
#define N_64 2<<5
#define N_512 2<<8


typedef struct {
    color_t **pixeles;
    size_t ancho, alto;
} imagen_t;

color_t color_desde_def(uint8_t d, uint8_t e, uint8_t f){
    color_t color = (d<<11) | (e<<5) | (f);
    return color;
}

uint8_t color_d(color_t c){
    uint8_t d = c >> 11;
    return d;
}

uint8_t color_e(color_t c){
    uint8_t e = (c >> 5) & MASK_E;
    return e;
}

uint8_t color_f(color_t c){
    uint8_t f = c & MASK_F;
    return f;
}

color_t color_desde_rgb(uint8_t r, uint8_t g, uint8_t b){
    uint8_t max, min,  d,e,f;
    uint8_t aux[3]={r,g,b};

    max=r;
    for(int i=1; i<3 ; i++)
        if(max<=aux[i])
            max=aux[i];
    
    min=r;
    for(int i=1; i<3 ; i++)
        if(aux[i]<=min)
            min=aux[i];

    f = (max+min)/(N_16);

    //encuentro D
    if(min==max)
        d=0;

    else if(f < (N_16))
        d =( ( ( (N_32) * (max-min))  /  (max+min) ) ) - 1 ;

    else if(f >= (N_16))
        d =  ( (N_32) * (max-min))  /  ( (N_512) - max - min );

    
    //encuentro E
    if(min==max)
        e=0;

    else if(max==r)
        e=( (11*(g-b)) / (max-min)  ) + (N_64);

    else if(max==g)
        e=( (11*(b-r)) / (max-min)  ) + 21;

    else if(max==b)
        e=( (11*(r-g)) / (max-min)  ) + 43;

    if(e >= (N_64)) 
        e = e - (N_64);
    
    color_t color=color_desde_def(d,e,f);
    
    return color;
}

void color_a_rgb(color_t c, uint8_t* r, uint8_t* g, uint8_t* b){

    uint8_t cc, x, m, h;
    uint8_t d=color_d(c), e=color_e(c), f=color_f(c);

    cc = ( 8 - fabs( (f/2) - 8) ) * d;

    float t = fmodf(e / 11.0, 2);
    

    x = (1 -  fabs( t - 1 ) ) * cc ; 
    m = 8*f - cc/2;
    h = e/11;

    uint8_t aux[3]; //r,g,b 

    if(h==0)
        aux[0]=cc,aux[1]=x,aux[2]=0 ;
    else if(h==1)
        aux[0]=x,aux[1]=cc,aux[2]= 0;
    else if(h==2)
        aux[0]=0,aux[1]=cc,aux[2]=x;
    else if(h==3)
        aux[0]=0,aux[1]=x,aux[2]=cc;
    else if(h==4)
        aux[0]=x,aux[1]=0,aux[2]=cc;
    else if(h==5)
        aux[0]=cc,aux[1]=0,aux[2]=x;
    
    //R,G,B

    *r = (m+aux[0])>255 ? 255 : m+aux[0];
    *g = (m+aux[1])>255 ? 255 : m+aux[1];
    *b = (m+aux[2])>255 ? 255 : m+aux[2];

}

//NUEVAS


color_t color_negro(void){
    return color_desde_rgb(0,0,0);
}

bool color_es_negro(color_t c){
    return ( (color_d(c) == 0) && (color_e(c) == 0) && (color_f(c) == 0));
}

bool mismo_color(color_t color1, color_t color2){
    return color_e(color1) == color_e(color2);
}

char dar_color(){
    char colores[] = {0, 11, 22, 44};
    return colores[rand() % 4];
}






















