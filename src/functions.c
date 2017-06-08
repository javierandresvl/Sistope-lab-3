#include "functions.h"

void call_getopt(int argc, char** argv, int *n, int *d, char **nombreArchivo){
	int c, hijos;
	char *Ivalue, *Nvalue, *Dvalue;


	//Obtener parametros.
	opterr = 0;

	if(argc <= 2){
		printf("Se deben ingresar opciones (uso: -I <nombre archivo de entrada> -N <ancho del tablero> -D)\n");
		exit(1) ;
	}

	while((c = getopt(argc, argv, "I:N:D:")) != -1){
		switch (c) {
			case 'I':
				Ivalue = optarg;
				break;
			case 'N':
				Nvalue = optarg;
				break;
			case 'D':
				Dvalue = optarg;
				break;
			case '?':
				if(optopt == 'I'){
					fprintf(stderr, "Opcion -I requiere un argumento.\n");
				}
				else if(optopt == 'N'){
					fprintf(stderr,"Opcion -N requiere un argumento\n");
				}
				else if(optopt == 'D'){
					fprintf(stderr,"Opcion -D requiere un argumento\n");
				}
				else if (isprint(optopt)) {
					fprintf(stderr, "Opcion -%c desconocida.\n", optopt);
				}
				else{
					fprintf(stderr, "Opcion desconocida '\\x%x'.\n", optopt);
				}
				exit(1);
			default:
				abort();
		}
	}

	//Procesar parametros.
	if(!isInt(Nvalue)){
		printf("El parametro ingresado para el tamano de la tabla no es valido\n");
		exit(1);
	}

	if(!isInt(Dvalue)){
		printf("El parametro ingresado para el debug no es valido\n");
		exit(1);
	}

	//retorno de parametros por referencia.
	*n = atoi(Nvalue);
	*nombreArchivo = Ivalue;
	*d = atoi(Dvalue);
}

int isInt(char *number){
 	int i, c;

 	for(i=0;i<strlen(number); i++){
    		c = number[i];
    		if(c < 48 || c > 57 || c == 0){
      			return 0;
    		}
  }
  return 1;
}

guerrero* readFile(char *name){
	guerrero* guerreros;
	int i, g, index;
	char c;
	int hp,  color, univ;
	char *nombre;
	FILE *file;

	file = fopen(name, "r");
	//Cantidad de filas = cantidad de guerreros
	g = 0;	//cantidad de filas
	while(!feof(file)){
		c = fgetc(file);
		if(c == '\n'){
			g++;
		}
	}
	rewind(file);//se devuelve el puntero del archivo al inicio

	//Se crean los guerreros
	guerreros = (guerrero*)malloc(sizeof(guerrero)*g);

	/*
	//Asignación de memoria
	hp = (char*)malloc(sizeof(char)*4);
	color = (char*)malloc(sizeof(char));
	univ = (char*)malloc(sizeof(char)*2);
	*/
	nombre = (char*)malloc(sizeof(char)*100);
	for(i=0;i<g;i++){
		fscanf(file, "%d %d %d %s", &hp, &color, &univ, nombre);
		//Colocar atributos al guerrero
		guerreros[i].hp = hp;
		guerreros[i].colorUniverso = color;
		guerreros[i].numUniverso = univ;
		guerreros[i].nombre = (char*)malloc(sizeof(char)*100);
		strcpy(guerreros[i].nombre, nombre);
		guerreros[i].fila = -1;
		guerreros[i].columna = -1;
		guerreros[i].numGuerrero = i;
	}
	//Se libera memoria
	free(nombre);


	fclose(file);

	cant_guerreros = g;

	return guerreros;
}

void resumen()
{
	int n;
	guerrero g;

	for(n = 0; n < cant_guerreros; n++)
	{
		g = guerreros[n];
		char* nombre = g.nombre;
		printf("Nombre guerrero: %s\nUniverso: %d\nColor universo:%d\nHit Points:%d\n\n", nombre, g.numUniverso, g.colorUniverso,g.hp);
	}

	printf("ENTER para continuar...\n");
	getchar();

	return;
}

void *pelear(void *arg)
{
	usleep(1);
	int flag1, flag2, flag3, m, n, flag_ocupado,  burbuja, ganador;
	int num_guerrero = (int *) arg;
	int ki = 0;
	char ki_s[100];
	char hp_s[100];
	int mover;
	int fila, columna;
	int fila_mover, columna_mover;
	int fila_enemigo, columna_enemigo, universo_enemigo;

	/*Primero ingreso el guerrero al tablero*/
	pthread_mutex_lock(&mutex);
	fila = rand() % tamanoTablero;
	columna = rand() % tamanoTablero;
	pthread_mutex_unlock(&mutex);

	/*Voy a modificar el tablero, así que cierro el mutex*/
	pthread_mutex_lock(&mutex);
	flag2 = 1;
	while(flag2 == 1)
	{
		flag1 = matriz[fila][columna];
		if(flag1 != 1)
		{
			flag2 = 0;
			matriz[fila][columna] = guerreros[num_guerrero].colorUniverso;
			guerreros[num_guerrero].fila = fila;
			guerreros[num_guerrero].columna = columna;
			contador_guerreros++;
			sprintf(hp_s,"%04d",guerreros[num_guerrero].hp);
			sprintf(ki_s,"%04d",ki);
			mvwprintw(score,num_guerrero+1,3,guerreros[num_guerrero].nombre);
			mvwprintw(score,num_guerrero+1,max_x/2-4,hp_s);
			mvwprintw(score,num_guerrero+1,max_x-8,ki_s);
			wrefresh(score);


			attron(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));
			move(fila+1,columna+1);
			addch(guerreros[num_guerrero].nombre[0]);
			attroff(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));
			refresh();


		}
		else
		{
			fila = rand() % tamanoTablero;
			columna = rand() % tamanoTablero;
		}
	}
	pthread_mutex_unlock(&mutex);

	/* Espero que todos los peleadores esten en el tablero */
	while(contador_guerreros != cant_guerreros){
	}

	pthread_mutex_lock(&mutex);
	c2 = 0;
	pthread_mutex_unlock(&mutex);

	fila_mover = guerreros[num_guerrero].fila;
	columna_mover = guerreros[num_guerrero].columna;

	/*
	Este ciclo indica hasta que momento el guerrero estará
	moviendose y peleando con las demás hebras.
	 */
	while(guerreros[num_guerrero].hp > 0)
	{
		usleep(10000);

		/*
		0: mover hacia arriba
		1: mover hacia la derecha
		2: mover hacia abajo
		3: mover hacia la izquierda
		*/

		pthread_mutex_lock(&mutex);

		mover = rand() % 4;
		//pthread_mutex_unlock(&mutex);

		flag_ocupado = 0;

		switch(mover)
		{
			case 0:
				fila_mover = guerreros[num_guerrero].fila - 1;
				break;
			case 1:
				columna_mover = guerreros[num_guerrero].columna + 1;
				break;
			case 2:
				fila_mover = guerreros[num_guerrero].fila + 1;
				break;
			case 3:
				columna_mover = guerreros[num_guerrero].columna - 1;
				break;
		}

		/*
		Ahorase verifica si es que se puede mover a la posicion
		asignada pseudo aleatoriamente.
		En caso de que no se pueda mover, se queda en el mismo
		espacio original.
		*/
		if((columna_mover >= 0) && (fila_mover >= 0) && (fila_mover < tamanoTablero) &&(columna_mover < tamanoTablero))
		{
			flag_ocupado = matriz[fila_mover][columna_mover];
		}
		else
		{
			fila_mover = fila;
			columna_mover = columna;
			flag_ocupado = -1;
		}
		if(flag_ocupado == 0)
		{
			matriz[fila_mover][columna_mover] = matriz[fila][columna];
			guerreros[num_guerrero].fila = fila_mover;
			guerreros[num_guerrero].columna = columna_mover;
			matriz[fila][columna] = 0;

			attron(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));
			mvaddch(fila_mover+1,columna_mover+1,guerreros[num_guerrero].nombre[0]);
			attroff(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));

			mvaddch(fila+1,columna+1,' ');

		}
		else{

			attron(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));
			mvaddch(fila+1,columna+1,guerreros[num_guerrero].nombre[0]);
			attroff(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));
		}

		c2++;

		if(c2 == cant_guerreros){
			refresh();
			c2 = 0;
		}

		pthread_mutex_unlock(&mutex);

		fila = fila_mover;
		columna = columna_mover;

		ki = ki + 1;
		pthread_mutex_lock(&mutex);
		sprintf(ki_s,"%04d",ki);
		mvwprintw(score,num_guerrero+1,max_x-8,ki_s);
		wrefresh(score);
		pthread_mutex_unlock(&mutex);

		/*
		Ahora veo a quien voy a golpear
		revisando que guerrero esta en la posicion adyacente
		*/

		pthread_mutex_lock(&mutex);
		for(m = 0; m < cant_guerreros; m++)
		{
			fila_enemigo = guerreros[m].fila;
			columna_enemigo = guerreros[m].columna;
			universo_enemigo = guerreros[m].numUniverso;

			/*
			Ahora veo si el guerrero es de un universo distinto,
			y además si esta en una casilla adyacente.
			*/
			if((guerreros[num_guerrero].numUniverso != universo_enemigo) && ((abs(guerreros[num_guerrero].columna - columna_enemigo) == 1) && (fila_enemigo == guerreros[num_guerrero].fila)) || ((abs(guerreros[num_guerrero].fila - fila_enemigo) == 1) && (columna_enemigo == guerreros[num_guerrero].columna)) )
			{
				guerreros[m].hp = guerreros[m].hp - (ki * 5);
				ki = 1;

				sprintf(ki_s,"%04d",ki);
				mvwprintw(score,num_guerrero+1,max_x-8,ki_s);
				wrefresh(score);

				attron(COLOR_PAIR(100 - guerreros[m].colorUniverso));
				mvaddch(fila_enemigo+1,columna_enemigo+1,guerreros[m].nombre[0]);
				attroff(COLOR_PAIR(100 - guerreros[m].colorUniverso));
				refresh();

				sprintf(hp_s,"%04d",guerreros[m].hp);
				mvwprintw(score,guerreros[m].numGuerrero + 1,max_x/2-4,hp_s);
				wrefresh(score);
			}

		}
		pthread_mutex_unlock(&mutex);

	}

	pthread_mutex_lock(&mutex);
	attron(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));
	mvaddch(guerreros[num_guerrero].fila+1,guerreros[num_guerrero].columna+1,' ');
	attroff(COLOR_PAIR(guerreros[num_guerrero].colorUniverso));
	refresh();

	guerreros[num_guerrero].fila = -10;
	guerreros[num_guerrero].columna = -10;

	sprintf(hp_s,"%04d",0);
	sprintf(ki_s,"%04d",0);
	mvwprintw(score,num_guerrero+1,3,"MUERTO");
	mvwprintw(score,num_guerrero+1,max_x/2-4,hp_s);
	mvwprintw(score,num_guerrero+1,max_x-8,ki_s);
	wrefresh(score);
	pthread_mutex_unlock(&mutex);

	/*
	Cuando se muere un peleador, hay que verificar
	si el juego ya ha terminado o no.
	*/

	pthread_mutex_lock(&mutex_ganador);
	ganador = finish();
	if(ganador != -1){
		deleteBoard();
		endwin();
		printf("GANO EL UNIVERSO: %d\n", ganador);
	}
	pthread_mutex_unlock(&mutex_ganador);

}

//Funciones pantalla.
void createBoard(){

	int score_size = 12;

	getmaxyx(stdscr, max_y, max_x);
	board = newwin(tamanoTablero + 2, tamanoTablero + 2, 0, 0);
	score = newwin(score_size, max_x, max_y - score_size, 0);
	wborder(board, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(score, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(board, 0, (tamanoTablero-2)/2, "BOARD");
	mvwprintw(score, 0, max_x/2-4, "HP");
	mvwprintw(score, 0, 3, "GUERRERO");
	mvwprintw(score,0, max_x-8, "KI");
	wrefresh(board);
	wrefresh(score);
}

void deleteBoard(){
	delwin(board);
	delwin(score);
}

void create_screen(){
	initscr();
	noecho();
	curs_set(FALSE);
	refresh();
	createBoard(board, score);

	if (has_colors())
	{
		start_color();
		/*
		* Asignación de colores simples, todos los necesarios normalmente.
		*/
		init_color(10,173,255,47);
		init_color(11,255,69,0);

		/* Colores normales */
		init_pair(1, 11, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		init_pair(3, COLOR_RED, COLOR_BLACK);
		init_pair(4, COLOR_CYAN, COLOR_BLACK);
		init_pair(5, COLOR_WHITE, COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(7, COLOR_BLUE, COLOR_BLACK);
		init_pair(8, COLOR_YELLOW, COLOR_BLACK);
		init_pair(9,10,COLOR_BLACK);

		/* Colores cuando uno es atacado */
		init_pair(99, 11, COLOR_RED);
		init_pair(98, COLOR_GREEN, COLOR_RED);
		init_pair(97, COLOR_RED, COLOR_RED);
		init_pair(96, COLOR_CYAN, COLOR_RED);
		init_pair(95, COLOR_WHITE, COLOR_RED);
		init_pair(94, COLOR_MAGENTA, COLOR_RED);
		init_pair(93, COLOR_BLUE, COLOR_RED);
		init_pair(92, COLOR_YELLOW, COLOR_RED);
		init_pair(91,10,COLOR_RED);
	}
}

int finish()
{
	int flag, m, last;
	flag = 1;
	guerrero g;
	last = 0;

	pthread_mutex_lock(&mutex);
	for(m = 0; m < cant_guerreros; m++)
	{
		g = guerreros[m];
		if(g.hp > 0)
		{
			if((last != g.numUniverso)&&(last != 0))
			{
				pthread_mutex_unlock(&mutex);
				return -1;
			}
			else
			{
				last = g.numUniverso;
			}
		}

	}
	pthread_mutex_unlock(&mutex);
	return last;
}
