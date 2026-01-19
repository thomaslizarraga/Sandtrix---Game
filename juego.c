
#include "imagen.h"
#include "juego.h"
#include <stdlib.h>
#include "config.h"


void expandir(color_t color,int fila, int columna,imagen_t* tablero, imagen_t* borrar, size_t alto_tablero, 
    size_t ancho_tablero){
    
    
    if( fila >= alto_tablero || columna >= ancho_tablero || fila < 0 || columna < 0)
        return ;

    else if( !color_es_negro(imagen_obtener_pixel(borrar,fila,columna)) ) //ya está marcado
        return;
    

    else if( !mismo_color(color, imagen_obtener_pixel(tablero, fila,columna))) //no es el color que busco
        return;
        
    else if( color_es_negro(imagen_obtener_pixel(tablero, fila,columna))) //no es el color que busco
        return;
    
    imagen_establecer_pixel(borrar, fila, columna, 0xff);
    
    expandir(color, fila, columna + 1, tablero, borrar, alto_tablero, ancho_tablero);
    expandir(color, fila - 1, columna, tablero, borrar, alto_tablero,ancho_tablero);
    expandir(color, fila + 1, columna, tablero, borrar, alto_tablero,ancho_tablero);
    expandir(color, fila, columna - 1 , tablero, borrar, alto_tablero,ancho_tablero);
    return;
}

bool pieza_no_existe(imagen_t* t){
     return (t==NULL)? true:false;
}

void remover_linea(imagen_t* tablero, imagen_t* borrar, size_t alto, size_t ancho, unsigned int* contador_puntaje){
    for(size_t i = 0; i<alto; i++)
        for(size_t j = 0; j<ancho; j++)
            if( imagen_obtener_pixel(borrar,i,j) == 0xff) {
                (*contador_puntaje)++;
                imagen_establecer_pixel(tablero,i,j,0);
            }

}

bool toque_alto(imagen_t* tablero){
    for(size_t i = 0; i < 80; i++)
        if( !color_es_negro(imagen_obtener_pixel(tablero,0,i)))
            return true;
    return false;
}

bool pieza_toca_arena(imagen_t* pieza, imagen_t* tablero, size_t fp, size_t cp){
    if(pieza == NULL) 
        return false;

    size_t alto  = imagen_alto(pieza);
    size_t ancho = imagen_ancho(pieza);

    // Recorrer todos los pixeles de la pieza para detectar colisión, pero desde la fila más baja.
    for(int i = (alto - 1); i >= 0; i--){
        for(int j = (ancho - 1) ; j >= 0; j--){

            color_t c = imagen_obtener_pixel(pieza, i, j);
            
            if( color_es_negro(c) ){
                continue;           
            }
            // Pixel inmediatamente debajo
            color_t abajo = imagen_obtener_pixel(tablero, fp + i + 1, cp + j);

            // Si lo de abajo NO es negro entonces chocó
            if( (!color_es_negro(abajo)) || (fp + alto)>=144){
                return true;
            }
        }
    }

    return false;
}

void simulacion_arena(imagen_t *tablero){
    
    size_t ancho = imagen_ancho(tablero);
    size_t alto = imagen_alto(tablero);
    
   imagen_t* tablero_nuevo = crear_tablero_negro(ALTO_TABLERO, ANCHO_TABLERO);

    //la ultima fila se copia tal cual está
    for(size_t i = 0; i < ancho; i++){
        color_t c = imagen_obtener_pixel(tablero, alto - 1, i);
        imagen_establecer_pixel(tablero_nuevo, alto - 1, i, c);
    }

    for(int i = (int)alto - 2; i >= 0; i--){
        for(size_t j = 0; j < (int)ancho; j++){

            color_t color_actual= imagen_obtener_pixel(tablero, i, j);
            if(color_es_negro(color_actual))
                continue;

            int r = rand() % 4;

            if(r != 0){ //Se mantiene el color
                imagen_establecer_pixel(tablero_nuevo, i, j, color_actual);
            }

            //intenta mover abajo
            else if( ((i + 1) < alto) && 
                color_es_negro( imagen_obtener_pixel(tablero, i + 1, j) ) &&  
                color_es_negro(imagen_obtener_pixel(tablero_nuevo, i + 1, j))){
                    imagen_establecer_pixel(tablero_nuevo, i + 1, j, color_actual);
                }
                
            //intenta mover en diagonales
            else{

                bool izq_libre = (j > 0) && ( (i+1) < alto) && color_es_negro(imagen_obtener_pixel(tablero, i + 1, j - 1)) &&
                color_es_negro(imagen_obtener_pixel(tablero_nuevo, i + 1, j - 1));
                bool der_libre = ( (j + 1) < ancho) && ( (i + 1) < alto) && color_es_negro(imagen_obtener_pixel(tablero, i + 1, j + 1)) &&
                color_es_negro(imagen_obtener_pixel(tablero_nuevo, i + 1, j + 1));

                if(izq_libre && der_libre){
                    if( (rand () % 2) == 0)
                        imagen_establecer_pixel(tablero_nuevo, i + 1, j - 1, color_actual);
                    else
                        imagen_establecer_pixel(tablero_nuevo, i + 1 , j + 1, color_actual);
                }

                else if(izq_libre)
                    imagen_establecer_pixel(tablero_nuevo, i + 1, j - 1, color_actual);
                else if (der_libre)
                    imagen_establecer_pixel(tablero_nuevo, i + 1, j + 1, color_actual);
                else
                    imagen_establecer_pixel(tablero_nuevo, i , j, color_actual);
            }
        }
    }
    
    for(size_t i = 0; i < alto; i++){
        for(size_t j = 0; j < ancho; j++){
            imagen_establecer_pixel(tablero,i, j, imagen_obtener_pixel(tablero_nuevo, i, j));
        }
    }
    imagen_destruir(tablero_nuevo);

}

bool se_genero_linea(size_t alto_tablero, size_t ancho_tablero, imagen_t* tablero, imagen_t* visitados, int fila, int columna, color_t color){
        
        if( fila >= alto_tablero || columna >= ancho_tablero || fila < 0 || columna < 0)
            return false;

        else if( ! color_es_negro(  imagen_obtener_pixel(visitados,fila,columna)   )  ) 
            return false;

        else if( ! mismo_color(color, imagen_obtener_pixel(tablero,fila,columna))){  
            return false;
        }

        else if( color_es_negro(imagen_obtener_pixel(tablero,fila,columna))){  
            return false;
        }

        else if(  (columna == (ancho_tablero - 1))){ //¿llegue al otro extremo?
            imagen_establecer_pixel(visitados, fila, columna, 0xff);//marco el pixel como visitado
            return true;
        }
        imagen_establecer_pixel(visitados, fila, columna, 0xff);//marco el pixel visitado
        
        return se_genero_linea(alto_tablero, ancho_tablero, tablero, visitados, fila, columna + 1, color ) || 
                
                se_genero_linea(alto_tablero, ancho_tablero, tablero,visitados , fila - 1 ,columna,color) || 
                
                se_genero_linea(alto_tablero, ancho_tablero, tablero, visitados, fila + 1 , columna,color) || 
                
                se_genero_linea(alto_tablero, ancho_tablero, tablero,visitados ,fila, columna - 1, color );  
                //me muevo en las 4 direcciones en priodidad: derecha, arriba, abajo, izquierda
    }

imagen_t* generar_miniatura(sprite_t* sprite){
    size_t ancho = sprite_ancho(sprite);
    size_t alto = sprite_alto(sprite);

    imagen_t* miniatura = imagen_crear(ancho, alto);
    if(miniatura == NULL)
        return NULL;
    for(size_t f = 0; f < alto; f++)
        for(size_t c = 0; c < ancho; c++){
            if(sprite_obtener(sprite,f,c)==1)
                imagen_establecer_pixel(miniatura,f,c,0xff);
            else
                imagen_establecer_pixel(miniatura,f,c,0);
        }
    return miniatura;    
}

imagen_t* generar_numeros(char* aux, sprites_t* sprites){

    imagen_t* contador = imagen_crear(ANCHO_FONDO_NUMEROS,ALTO_FONDO_NUMEROS); //tamaño de la parte en negro donde van numeros
    for(size_t i = 0; i < ALTO_FONDO_NUMEROS; i++)
        for(size_t j = 0; j< ANCHO_FONDO_NUMEROS; j++)
            imagen_establecer_pixel(contador,i,j,0);

    if(contador==NULL){
        fprintf( stderr,"No se puede generar imagen contador\n");
        return NULL;
    }
    
    size_t fila_numero = 0;
    size_t columna_numero = 0;

    for (int i = 0; aux[i] != '\0'; i++) {
        char tmp[2];
        tmp[0] = aux[i];  
        tmp[1] = '\0';

        sprite_t* numero = sprites_obtener(sprites, tmp);
        if(numero == NULL){
            fprintf(stderr,"El numero no existe");
            printf("%c\n",aux[i]);
            imagen_destruir(contador);
            return NULL;
        }
        size_t alto = sprite_alto(numero);
        size_t ancho = sprite_ancho(numero);

        imagen_t* digito = imagen_crear(ancho,alto);

        for(size_t i = 0; i < alto; i++)
            for(size_t j=0; j < ancho; j++){
                if(sprite_obtener(numero,i,j) )
                    imagen_establecer_pixel(digito,i,j,0xff);
                else
                    imagen_establecer_pixel(digito,i,j,0);
            }
            
        pegar_subimagen_tranparencia(contador, digito, fila_numero, columna_numero);
        columna_numero = columna_numero + ancho;
        imagen_destruir(digito);
    }

    return contador;
}

imagen_t* generar_tiempo(unsigned int ticks, sprites_t* sprites){
    
    char minutos = ticks/60000;
    char segundos = (ticks%60000)/1000;
    unsigned char milisegundos = ticks % 1000;

    char aux[50];
    snprintf(aux,sizeof(aux),"%02d:%02d:%02d",minutos,segundos, milisegundos);
    imagen_t* tiempo = generar_numeros(aux, sprites);
    return tiempo;
}

void generar_puntaje(unsigned int contador_puntaje, imagen_t** puntos, unsigned int contador_clears, imagen_t** clears, sprites_t* sprites){
    
    char aux[50];
    
    snprintf(aux,sizeof(aux),"%d",contador_puntaje);
    *puntos = generar_numeros(aux, sprites);
    
    snprintf(aux,sizeof(aux),"%d",contador_clears);
    *clears = generar_numeros( aux, sprites);

}

imagen_t* pieza_crear(color_t color, sprite_t* sprite){
    size_t ancho = sprite_ancho(sprite);
    size_t alto = sprite_alto(sprite);

    imagen_t* pieza = imagen_crear(ancho, alto);
                
    for(size_t i=0; i < alto; i++){
        for(size_t j=0; j < ancho; j++){
            if(sprite_obtener(sprite, i, j)==0) //establecer color negro
                imagen_establecer_pixel(pieza,i,j,0);
                            
            else //establecer color e
                imagen_establecer_pixel(pieza,i,j,color_desde_def( 12 + (rand()%8), color , 12 + (rand()%8) ));
            }
    }
    return pieza;
}

imagen_t* generar_tubo(sprites_t* sprites, color_t color){
    char* tubo_etiqueta = "tubo";
    sprite_t* sprite_tubo = sprites_obtener(sprites,tubo_etiqueta);
    imagen_t* tubo = pieza_crear(color, sprite_tubo);
    return tubo;
}

imagen_t* crear_tablero_negro(){
    imagen_t* tablero = imagen_crear(ANCHO_TABLERO,ALTO_TABLERO);
    if(tablero==NULL){
        fprintf(stderr,"No se pudo crear tablero en negro\n");
        return NULL;
    }
    for(size_t i=0; i < 144; i++)
        for(size_t j=0; j < 80; j++)
            imagen_establecer_pixel(tablero, i, j , 0);
    return tablero;
}


void desplazamientos(char direccion, size_t* cp, char* desplazamiento_restante){
    *cp = *cp + direccion;
    (*desplazamiento_restante)--;
}

void aumento_de_velocidad(unsigned int ticks, unsigned int* ticks_previo, unsigned int* tiempo_aceleracion,
    float* suma_1_segundos ,float* velocidad, float* f_, 
    bool* aceleracion_activada){

    if (*aceleracion_activada) {
        if ( (ticks - *tiempo_aceleracion) >= ACELERACION_DURACION) {
            *aceleracion_activada = false;
        }
    }

    *suma_1_segundos = *suma_1_segundos + (ticks - (*ticks_previo) )/1000.0f;

    *ticks_previo = ticks;

    if( (*suma_1_segundos) >= 1){
        *velocidad = *velocidad + ( (*velocidad) * 0.01);
        *suma_1_segundos = *suma_1_segundos - 1;
    }

    float velocidad_efectiva = *velocidad;

    if (*aceleracion_activada) {
        velocidad_efectiva = velocidad_efectiva * AUMENTO_DE_VELOCIDAD;
    }

    *f_ = *f_ + velocidad_efectiva/JUEGO_FPS;

}

char* dar_pieza_random(){
    char* piezas[]={"i", "j", "l" , "o" , "s" , "t" , "z"};
    return piezas[rand()%7];
}

//opcional 
void puntaje_en_pantalla(unsigned int contador_puntaje, unsigned int contador_clears){
    puts("GAME OVER");
    printf("Puntos totales = %d\nClears = %d\n", contador_puntaje, contador_clears);
}