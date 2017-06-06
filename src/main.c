#include "functions.h"

/*
  Laboratorio 3 - Sistemas operativos
  USACH 1-2017
  Integrantes:
  - Kevin Alvarez
  - Javier Vasquez
  Descripción:
  El presente laboratorio consiste en la creación de hebras, para
  recrear el torneo de fuerza de DBS.
*/

int main(int argc, char** argv)
{

	int n,m;
	int Dflag;
	char *nombreArchivo = (char*)malloc(sizeof(char) * 100);

	/*  Realizo el get opt de los valores de entrada hechos por la consola */
	call_getopt(argc, argv, &tamanoTablero, &Dflag, &nombreArchivo);

	/* inicio ciertas variables globales, como el arreglo de los guerreros */
	cant_guerreros = 0;
	contador_guerreros = 0;
	guerreros = readFile(nombreArchivo);
	matriz = (int**)malloc(sizeof(int*) * tamanoTablero);
	for (n = 0; n < tamanoTablero; n++)
	{
		matriz[n] = (int*)malloc(sizeof(int) * tamanoTablero);
	}
	for(n = 0; n < tamanoTablero; n++)
	{
		for(m = 0; m < tamanoTablero; m++)
		{
			matriz[n][m] = 0;
		}
	}


	/* Verifico el valor de la opcion debug*/
	if(Dflag == 1)
	{
		resumen();
	}

	/* Creo la ventana con el tablero de juego */
	create_screen();

	/*
	mvwprintw(board, 1, 5, "B");
	wrefresh(board);
	getchar();
	*/

	/* Creo el mutex para que no exista condicion de carrera en la matriz*/
	pthread_mutex_init(&mutex, NULL);

	srand(time(NULL)); 
	pthread_t hebra[cant_guerreros];
	for(n = 0; n < cant_guerreros; n++)
	{
		pthread_create(&hebra[n], NULL, pelear, (void *) n);
	}

	for(n = 0; n < cant_guerreros; n++)
	{
		pthread_join(hebra[n], NULL);
	}


	getchar();

	deleteBoard(board, score);
	endwin();

	return 0;
}
