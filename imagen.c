#include "imagen.h"
#include "color.h"
#include "sprites.h"
#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>


struct imagen{
    color_t **pixeles; //colot_t es uint8_t
    size_t ancho, alto;
};

//EJ1 Y EJ2

void inicializar_matriz(size_t filas, size_t columnas, char matriz[filas][columnas]){
    for (size_t f=0; f<filas; f++)
        for(size_t c=0; c<columnas; c++ )
            matriz[f][c]=0;
}

void copiar_matriz(size_t filas, size_t columnas, char destino[filas][columnas], char origen[filas][columnas]){
    for(size_t f=0; f<filas; f++)
        for(size_t c=0; c<columnas; c++)
                destino[f][c] = origen[f][c];
}

void imprimir_ppm(size_t filas, size_t columnas, char matriz[filas][columnas]){
    printf("P1\n%zd %zd\n", filas, columnas);
    for(int i=0; i<filas ;i++){
        for(int j=0; j<columnas; j++){
            putchar( matriz[i][j] == 0 ? '0':'1');
            putchar(' ');
        }
        printf("\n");
    }
}

bool pegar_submatriz(
    size_t mfs, size_t mcs, char matriz[mfs][mcs], 
    size_t sfs, size_t scs, char submatriz[sfs][scs], 
    size_t fi, size_t ci){

    if( (fi+sfs-1) < mfs && (ci+scs-1) < mcs){
        for (int f=0; f<sfs; f++)
            for(int c=0; c<scs; c++)
                matriz[f+fi][c+ci] = submatriz[f][c];
        return true;
    }
    return false;

}



int sumar_entorno(size_t filas, size_t columnas, char matriz[filas][columnas], size_t fc, size_t cc){
    if ( ( 0 < fc && fc < filas-1) && ( 0 < cc && cc < columnas-1)){
        int suma=0;
        for(size_t f=0; f<3; f++)
            for(size_t c=0; c<3; c++)
                suma = suma + matriz[fc-1+f][cc-1+c];
    return suma - matriz[fc][cc];   //le quitas el valor de la celula misma a la suma    
    }
    else
        return 0;
}

void evolucionar_celulas(size_t filas, size_t columnas, char final[filas][columnas], char inicial[filas][columnas]){
    for(size_t f=0; f<filas-2; f++){
        for(size_t c=0; c<columnas-2; c++){
            int suma = sumar_entorno(filas,columnas,inicial,f+1,c+1);

            final[f+1][c+1] = (inicial[f+1][c+1] == 0 && suma == 3)? 1:0;

            if (inicial[f+1][c+1] == 1){
                final[f+1][c+1] = (suma == 2 || suma == 3)? 1:0;
            }
        }
    }
}

imagen_t *imagen_crear(size_t ancho, size_t alto) {
    imagen_t *imag = malloc(sizeof(imagen_t));
    if (imag == NULL) return NULL;

    imag->pixeles = malloc(alto * sizeof(color_t *));
    if (imag->pixeles == NULL) {
        free(imag);
        return NULL;
    }

    imag->ancho = ancho;
    imag->alto = alto;

    for (size_t i = 0; i < alto; i++) {
        imag->pixeles[i] = malloc(ancho * sizeof(color_t));
        if (imag->pixeles[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(imag->pixeles[j]);
            free(imag->pixeles);
            free(imag);
            return NULL;
        }
    }

    return imag;
}

void imagen_destruir(imagen_t *imag) {
    if (imag == NULL) return;
    for (size_t i = 0; i < imag->alto; i++) {
        free(imag->pixeles[i]);
    }
    free(imag->pixeles);
    free(imag);
}

imagen_t* copiar_imagen(const imagen_t* i){
    size_t clon_ancho = i->ancho, clon_alto = i->alto;
    imagen_t* clon=imagen_crear(clon_ancho,clon_alto);
    
    if(clon==NULL)
        return NULL;

    for(size_t f=0; f<clon_alto; f++)
        for(size_t c=0; c<clon_ancho; c++)
            clon->pixeles[f][c] = i->pixeles[f][c];

    return clon;
}

void imagen_escribir_ppm(const imagen_t* i){
    puts("P3");
    printf("%zd %zd\n",i->ancho,i->alto);
    puts("255");
    for(size_t f=0; f < i->alto; f++)
        for(size_t c=0; c < i->ancho; c++){
            uint8_t r,g,b;
            color_a_rgb(i->pixeles[f][c],&r,&g,&b);

            printf("%d\n%d\n%d\n",r,g,b);
        }
    
}

imagen_t* imagen_leer_ppm(FILE* a){
    
    char linea_1[10];
    fgets(linea_1,10,a);
    
    
    if( strcmp(linea_1,"P3\n") != 0 ){
        puts("No se puede leer primera linea");
        return NULL;
    }

    char caracter;
    size_t ancho=0, alto=0;
    char alto_ancho[10]={0};
    size_t i=0, j=0;
    char aux[100];
    

    //while para saltear los primeros comentarios y extraer el ancho y el alto
    while( strcmp(fgets(aux,100,a),"255\n") != 0 ){
        
        if( aux[0] == '#' )
            continue;

        while(  (caracter=aux[i]) != '\n' ){
            if( isblank(caracter) ){
                ancho=atoi(alto_ancho);
                i++;
                j=0;
                continue;
            }
            alto_ancho[j]=caracter;
            i++,j++;
        }
        alto=atoi(alto_ancho);
    }

    imagen_t* imagen=imagen_crear(ancho,alto);

    //while para los valores RGB
    
    uint8_t rgb[3];
    for(size_t f=0; f<alto; f++){
        for(size_t c=0; c<ancho; c++){
            size_t t=0;
            while(t<3){
                
                fgets(aux,100,a);
                if(aux[0] != '#'){
                    rgb[t] = atoi(aux);
                    t++;
                }
            }
            imagen->pixeles[f][c] = color_desde_rgb(rgb[0],rgb[1],rgb[2]);
        }
        
    }
    
    return imagen;
}


//NUEVAS
bool pegar_subimagen_tranparencia(imagen_t* imagen, imagen_t* subimagen, int posicion_f, int posicion_c){

    //analizar casos de borde.
    if( (posicion_f + imagen_alto(subimagen) ) > imagen_alto(imagen) || 
        (posicion_f < 0) || 
        (posicion_c + imagen_ancho(subimagen) ) > imagen_ancho(imagen) ||
        posicion_c < 0){
        
        return false;
    }

    for( size_t f=0; f < subimagen->alto; f++)
        for(size_t c=0; c< subimagen->ancho; c++){
            if (!color_es_negro(subimagen->pixeles[f][c]) )
                imagen->pixeles[f+posicion_f][c+posicion_c] = subimagen->pixeles[f][c];
            
        }

    return true;


}

size_t imagen_alto(imagen_t* i){
    return i->alto;
}

size_t imagen_ancho(imagen_t* i){
    return i->ancho;
}

imagen_t *imagen_rotar_antihorario (const imagen_t *original){
    if(!original)return NULL;

    size_t alto = original -> ancho;
    size_t ancho = original -> alto;

    imagen_t *rotada = imagen_crear(ancho,alto);
    if(!rotada)return NULL;

    for(size_t i = 0; i < alto; i++){
        for(size_t j = 0; j < ancho; j++){
                rotada -> pixeles[i][j] = original -> pixeles [j][alto - 1 - i];
        }

    }
    return rotada;
}

void imagen_establecer_pixel(imagen_t *imag, size_t fil, size_t col, color_t c){
    if(!imag)return;//verfico que el puntero exista

    if (fil >= imag->alto || col >= imag->ancho) return; //verifico limites

    imag -> pixeles[fil][col] = c;//le asigno un color al pixel

}

color_t imagen_obtener_pixel(const imagen_t *imagen, int fil, int col){
    //if(!imagen)
      //  return color_negro();

    if(fil >= imagen->alto || col >=  imagen->ancho || col < 0 || fil < 0)
        return color_negro();

    return imagen->pixeles[fil][col];
}

imagen_t* crear_tablero_negro(){
    imagen_t* tablero = imagen_crear(80,144);
    if(tablero==NULL){
        fprintf(stderr,"No se pudo crear tablero en negro\n");
        return NULL;
    }
    for(size_t i=0; i < 144; i++)
        for(size_t j=0; j < 80; j++)
            tablero->pixeles[i][j] = 0;
    return tablero;
}



