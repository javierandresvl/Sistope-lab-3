#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include <time.h>

//////// --ESTRUCTURAS-- ///////
typedef struct{
  int hp;
  int colorUniverso;
  int numUniverso;
  char* nombre;
  int fila;
  int columna;
  int numGuerrero;
}guerrero;

//////// -- VAR. GLOBALES -- ///////
int cant_guerreros;
guerrero* guerreros;
pthread_mutex_t mutex, mutex_ganador;
int contador_guerreros, c2;
int **matriz;
int tamanoTablero;
WINDOW *board, *score;
int max_x, max_y;

//////// --FUNCIONES-- ////////

/*
    Procedimiento que hace la llamada a getopt para obtener las opciones y
    parametros ingresados por la terminal.
    @param argc cantidad de parametros ingresados por consola
    @param argv arreglo con los parametros ingresados por consola
    @param n paso por referencia para obtener el tamaño del tablero
    @param d paso por referencia para obtener la bandera para mostrar el resumen de los guerreros
    @param nombreArchivo paso por referencia para obtener el nombre del archivo ingresado
*/
void call_getopt(int argc, char** argv, int *n, int *d, char **nombreArchivo);

/*
    Funcion que hace la lectura del archivo con los guerreros y retorna éstos en un arreglo.
    @param name nombre del archivo donde están los guerreros
    @return arreglo con los guerreros leídos
*/
guerrero* readFile(char* name);

/*
    Funcion que verifica si un string representa un numero o no, entrega 0 si es
    un numero y 1 si no.
    @param number string para verificar.
    @return resultado de la verificacion.
*/
int isInt(char *number);

/*
    Funcion que entrega el resumen de los guerreros en caso de que se
    le solicite al programa.
*/
void resumen();

/*
    Procedimiento que realiza todas las intrucciones (incluyendo las de visualización)
    para que los guerreros luchen, en el programa cada hebra es un guerrero, por ende
    ésta es el callback que se le pasa a cada hebra, y que esta ejecuta cuando inicia.
    @param arg argumento de la función, para este caso se pasa el número del guerrero
               que estará asignado a esa hebra.
*/
void *pelear(void *arg);

/*
    Procedimiento que comprueba si la pelea ha terminado, para esto comprueba el hp de los
    guerreros, si queda solo uno con hp mayor a 0, retorna el valor para finalizar la pelea.
*/
int finish();

//////// --FUNCIONES PARA PANTALLA-- ////////

/*
    Procedimiento que usando la librería ncurses crea la ventana (base) donde se va a
    visualizar la pelea entre los guerreros.
*/
void create_screen();

/*
    Procedimiento que crea las ventanas del tablero y el puntaje con un diseño, para poder
    ser visualizadas en pantalla.
*/
void createBoard();

/*
    Procedimiento que cierra las ventanas abiertas, en este caso, la ventana del tablero y
    la del puntaje.
*/
void deleteBoard();

#endif
