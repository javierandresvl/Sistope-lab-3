#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

//////// --VARIABLES GLOBALES-- ///////


//////// --FUNCIONES-- ////////

/*
   Procedimiento que hace la llamada a getopt para obtener las opciones y
   parametros ingresados por la terminal.
   @parametros ...
*/
void call_getopt(int argc, char** argv, int *n, int *d, char **nombreArchivo);

/*
   Funcion que verifica si un string representa un numero o no, entrega 0 si es
   un numero y 1 si no.
   @param number string para verificar.
   @return resultado de la verificacion.
*/
int isInt(char *number);

#endif