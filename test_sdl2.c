#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "color.h"
#include "config.h"
#include "imagen.h"
#include "sprite.h"
#include "sprites.h"
#include "juego.h"


int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Sandtrix");

    SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, VENTANA_ANCHO, VENTANA_ALTO);
    uint32_t canvas[VENTANA_ALTO * VENTANA_ANCHO];

    int dormir = 0;

// BEGIN código del alumno
    //debemos cargar todos los componentes del juego
    FILE *f = fopen("fondo.ppm", "rt");
    if(f==NULL){
        fprintf(stderr,"No se puede abrir el archivo\n");
        return 1;
    }
    imagen_t *im = imagen_leer_ppm(f); //se elimina a lo ultimo
    if(im==NULL){
        fclose(f);
        fprintf(stderr,"No se puede leer la imagen\n");
        return 1;
    }
    fclose(f);
    int fi = 0;
    int ci = 0;
    //fin de cargado de fondo

    //inicializo tableros
    imagen_t *tablero = crear_tablero_negro();
    if(tablero==NULL){
        fprintf(stderr,"No se pudo crear tablero\n");
        imagen_destruir(im);
        return 1;
    }
    imagen_t *tablero_visitados = crear_tablero_negro();
    if(tablero_visitados==NULL){
        fprintf(stderr,"No se puedo crear tablero de visitados\n");
        imagen_destruir(im);
        imagen_destruir(tablero);
        return 1;
    }
    //fin de inicializar tableros

    //cargamos los sprites
    sprites_t* sprites=sprites_crear("sprites.bin"); //se elimina al final
    if(sprites==NULL){
        imagen_destruir(im);
        imagen_destruir(tablero);
        imagen_destruir(tablero_visitados);
        fprintf(stderr,"No se pudo carga sprites\n");
        return 1;
    }
    
    // inicializa la semilla de números aleatorios
    srand(time(NULL));  

    imagen_t* pieza = NULL;
    imagen_t* pieza_siguiente = NULL;
    imagen_t* miniatura = NULL; 
    imagen_t* tubo = NULL;
    imagen_t* copia = NULL;
    char color_pieza_siguiente ;
    
    size_t fp = CENTRO_FILA_TABLERO;
    float f_ = 0.0;
    size_t cp = CENTRO_COLUMNA_TABLERO; 
    
    //inicializamos cosas de velocidad
    size_t diferencia_entre_filas = 0;
    float velocidad = JUEGO_FPS;
    float suma_1_segundos = 0.0;
    unsigned int ticks_previo = 0;

    //inicializamos contadores
    unsigned int contador_puntaje = 0;
    unsigned int contador_clears = 0;

    imagen_t* puntos = NULL;
    imagen_t* clears = NULL;
    
    generar_puntaje(contador_puntaje, &puntos, contador_clears, &clears, sprites);

    if(puntos==NULL){
        fprintf(stderr,"No se puede generar puntos\n");
        imagen_destruir(im);
        imagen_destruir(tablero);
        imagen_destruir(tablero_visitados);
        sprites_destruir(sprites);
        return 1;
    }
    if(clears==NULL){
        fprintf(stderr,"No se puede generar clears\n");
        imagen_destruir(im);
        imagen_destruir(tablero);
        imagen_destruir(tablero_visitados);
        imagen_destruir(puntos);
        sprites_destruir(sprites);
        return 1;
    }

    imagen_t* tiempo = NULL;

    //desplazamientos
    char desplazamiento_restante = 0;   // cuántos píxeles faltan mover
    char direccion = 0;                // -1 izquierda, +1 derecha
    //desplazamiento hacia abajo
    bool aceleracion_activada = false;
    unsigned int tiempo_aceleracion = 0;

// END código del alumno

    unsigned int ticks = SDL_GetTicks();
    while(1) {   
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                break;
            }
//BEGIN código del alumno
            if (event.type == SDL_KEYDOWN) {  // Si se apretó una tecla
                switch(event.key.keysym.sym) {
                   case SDLK_UP: {
                        imagen_t* rotada = imagen_rotar_antihorario(pieza);
                        //mantengo centro
                        size_t centro = cp + imagen_ancho(pieza) / 2;
                        cp = centro - imagen_ancho(rotada) / 2;
                        //casos de bordes
                        if (cp < LIMITE_BORDE_IZQUIERDO) {
                            cp = POSICION_COLUMNA_TABLERO;
                        }
                        if (cp + imagen_ancho(rotada) > LIMITE_BORDE_DERECHO) {
                            cp = LIMITE_BORDE_DERECHO - imagen_ancho(rotada);
                        }
                        imagen_destruir(pieza);
                        pieza = rotada;
                        break;
                    }
                    case SDLK_DOWN:{
                        if(!aceleracion_activada){
                            aceleracion_activada = true;
                            tiempo_aceleracion = ticks;
                        }
                        break;
                    }
                    case SDLK_RIGHT:{
                        if( (desplazamiento_restante == 0) && (cp + imagen_ancho(pieza)) < LIMITE_BORDE_DERECHO){
                            desplazamiento_restante = DESPLAZAMIENTOS_RESTANTES;
                            direccion = DESPLAZAMIENTO_RIGHT;
                        }
                        break;
                    }
                    case SDLK_LEFT:{
                        if( (cp > LIMITE_BORDE_IZQUIERDO) && (desplazamiento_restante == 0)){
                            desplazamiento_restante = DESPLAZAMIENTOS_RESTANTES;
                            direccion = DESPLAZAMIENTO_LEFT;
                        }
                        break;
                    }
                }
            }
 //END código del alumno
            continue;
        }

// BEGIN código del alumno
        
        copia = copiar_imagen(im); //ACLARACION: No tenía intencion de hacer esta copia, 
        //pero me sirvió para la miniatura de la pieza siguiente

        //creo una pieza, pero tiene que perdurar
        if(pieza_no_existe(pieza)){
            pieza = pieza_crear(dar_color(), sprites_obtener(sprites, dar_pieza_random() ) );
            if(pieza == NULL){
                fprintf(stderr,"Está fallando la creacion de la pieza actual\n");
                break;
            }
            cp = CENTRO_COLUMNA_TABLERO - imagen_ancho(pieza)/2;
        }
        //fin de creacion de pieza
        //creo una pieza siguiente, pero tiene que perdurar
        if(pieza_no_existe(pieza_siguiente)){
            char* pieza_etiqueta = dar_pieza_random();

                    char aux[5]; //estas 4 lineas son para la miniatura
                    sprintf(aux,"%sc", pieza_etiqueta);
                    imagen_destruir(miniatura);
                    miniatura = generar_miniatura( sprites_obtener(sprites, aux) );

            color_pieza_siguiente = dar_color();
            pieza_siguiente = pieza_crear(color_pieza_siguiente, sprites_obtener(sprites, pieza_etiqueta) );
            if(pieza_siguiente == NULL){
                fprintf(stderr,"Está fallando la creacion de la pieza siguiente\n");
                break;
            }
            cp = CENTRO_COLUMNA_TABLERO - imagen_ancho(pieza)/2;

        }
        //fin de creacion de pieza siguiente
        tubo = generar_tubo(sprites,color_pieza_siguiente);
        if(tubo == NULL){
            fprintf(stderr,"Está fallando la creacion del tubo\n");
            break;
        }

        //para desplazamientos izquierda o derecha
        if ( desplazamiento_restante > 0) {
            desplazamientos(direccion, &cp, &desplazamiento_restante);
            if ((cp + imagen_ancho(pieza) >= LIMITE_BORDE_DERECHO) || (cp < LIMITE_BORDE_IZQUIERDO))
                desplazamiento_restante = 0;
        }

        aumento_de_velocidad(ticks, &ticks_previo, &tiempo_aceleracion ,&suma_1_segundos, &velocidad, &f_, &aceleracion_activada);
        
        if(  (size_t)f_  !=  fp ) {
            
            diferencia_entre_filas = (size_t)f_ - fp;
            //toco arena?
            for(size_t k = 0; k < diferencia_entre_filas; k++){
                if(pieza_toca_arena(pieza,tablero, fp+k, cp-POSICION_COLUMNA_TABLERO)){ //si toca arena o llega al final del tablero
                    
                    pegar_subimagen_tranparencia(tablero, pieza, fp + k, cp - POSICION_COLUMNA_TABLERO);
                    imagen_destruir(pieza); 
                    pegar_subimagen_tranparencia(copia,tablero,POSICION_FILA_TABLERO,POSICION_COLUMNA_TABLERO);
                    
                    fp = CENTRO_FILA_TABLERO;
                    f_ = 0;
                    
                    pieza = pieza_siguiente;
                    pieza_siguiente = NULL;
                    
                    break;
                }
                else if ( k == (diferencia_entre_filas - 1) ){
                    pegar_subimagen_tranparencia(copia,tablero,POSICION_FILA_TABLERO,POSICION_COLUMNA_TABLERO);
                    pegar_subimagen_tranparencia(copia,pieza, fp + k, cp);
                }
            }
            if(toque_alto(tablero)){
                imagen_destruir(copia);  
                imagen_destruir(tubo);
                puntaje_en_pantalla(contador_puntaje, contador_clears); //opcional
                break;
            }
            fp = f_ ; //actualizo la nueva posición.
        }
        //pego cosas en el tablero
        tiempo = generar_tiempo(ticks,sprites);
        pegar_subimagen_tranparencia(copia,tiempo,TIEMPO_FILA, TIEMPO_COLUMNA);
        pegar_subimagen_tranparencia(copia,miniatura, CENTRO_FILA_MINIATURA - imagen_alto(miniatura)/2 , CENTRO_COLUMNA_MINIATURA - imagen_ancho(miniatura)/2);
        pegar_subimagen_tranparencia(copia,tubo,TUBO_FILA,TUBO_COLUMNA);
        pegar_subimagen_tranparencia(copia,puntos,PUNTOS_FILA,PUNTOS_COLUMNA);
        pegar_subimagen_tranparencia(copia,clears, CLEARS_FILA, CLEARS_COLUMNA);
        
        //simulo la arena 3 veces 
        for(size_t i = 0; i < CANTIDAD_SIMULACIONES ; i++){
            simulacion_arena(tablero); 
            //tengo que verificar en cada iteracion si no se formó una linea.
            //detectar linea:
            for(int f = ALTO_TABLERO - 1; f >=0; f--){ //analizo toda la primera fila desde abajo hacia arriba
                color_t color_actual = imagen_obtener_pixel(tablero, f, 0);

                if( color_es_negro(color_actual) )
                    continue;

                else if(se_genero_linea(ALTO_TABLERO,ANCHO_TABLERO,tablero,tablero_visitados, f,0,color_actual)){
                    contador_clears++;
                    imagen_destruir(tablero_visitados);
                    
                    tablero_visitados = crear_tablero_negro();
                    expandir(color_actual,f,0,tablero,tablero_visitados,ALTO_TABLERO,ANCHO_TABLERO);
                    remover_linea(tablero,tablero_visitados,ALTO_TABLERO,ANCHO_TABLERO,&contador_puntaje);

                    pegar_subimagen_tranparencia(copia,tablero_visitados,POSICION_FILA_TABLERO,POSICION_COLUMNA_TABLERO);
                   
                    imagen_destruir(puntos); 
                    imagen_destruir(clears); 
                    
                    generar_puntaje(contador_puntaje, &puntos, contador_clears, &clears, sprites);
                    }
                imagen_destruir(tablero_visitados);
                tablero_visitados = crear_tablero_negro();
                }
            
            }

        //esto es todo el conjunto: pieza, tablero, fondo, etc. No tocar nada por el momento.
        memset(canvas, 0, VENTANA_ALTO * VENTANA_ANCHO * sizeof(uint32_t));
        for(size_t f = 0; f < imagen_alto(copia); f++)
            for(size_t c = 0; c < imagen_ancho(copia); c++) {
                color_t cc = imagen_obtener_pixel(copia, f, c);
                uint8_t r, g, b;
                color_a_rgb(cc, &r, &g, &b);
                if( (fi + f) * 4 >= VENTANA_ALTO || (ci + c) * 4 >= VENTANA_ANCHO) 
                    continue;
            
                for(size_t i = 0; i < 4; i++)
                    for(size_t j = 0; j < 4; j++)
                        canvas[((fi + f) * 4 + i) * VENTANA_ANCHO + (ci + c) * 4 + j] = r << 24 | g << 16 | b << 8;
            };
            imagen_destruir(copia); 
            imagen_destruir(tiempo); 
            imagen_destruir(tubo); 

// END código del alumno
        SDL_UpdateTexture(texture, NULL, canvas, VENTANA_ANCHO * sizeof(uint32_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        else
            printf("Perdiendo cuadros\n");
        ticks = SDL_GetTicks();
    }

// BEGIN código del alumno
    //Destruir todo lo que haya que destruir: tablero, piezas, etc.
    imagen_destruir(im); 
    imagen_destruir(pieza);
    imagen_destruir(pieza_siguiente);
    imagen_destruir(tablero);
    imagen_destruir(tablero_visitados);
    imagen_destruir(puntos);
    imagen_destruir(miniatura); 
    imagen_destruir(clears);
    sprites_destruir(sprites); 
    
// END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}