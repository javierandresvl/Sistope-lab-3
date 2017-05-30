#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>


//////// --ESTRUCTURAS-- ///////
typedef struct{
  int hp;
  int colorUniverso;
  int numUniverso;
  char* nombre;
}guerrero;


//////// --FUNCIONES-- ////////

/*
   Procedimiento que hace la llamada a getopt para obtener las opciones y
   parametros ingresados por la terminal.
   @parametros ...
*/
void call_getopt(int argc, char** argv, int *n, int *d, char **nombreArchivo);

guerrero* readFile(char* name);

//////// --FUNCIONES PARA PANTALLA-- ////////
void create_screen();

void createBoard(WINDOW *board, WINDOW *score);

void deleteBoard(WINDOW *board, WINDOW *score);
/*
   Funcion que verifica si un string representa un numero o no, entrega 0 si es
   un numero y 1 si no.
   @param number string para verificar.
   @return resultado de la verificacion.
*/
int isInt(char *number);

#endif
