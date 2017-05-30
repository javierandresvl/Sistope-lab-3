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
	char *hp, *color, *univ, *nombre;
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
	guerreros = (guerrero*)malloc(sizeof(guerrero)*i);
	//Asignación de memoria
	hp = (char*)malloc(sizeof(char)*4);
	color = (char*)malloc(sizeof(char));
	univ = (char*)malloc(sizeof(char)*2);
	nombre = (char*)malloc(sizeof(char)*100);
	for(i=0;i<g;i++){
		fscanf(file, "%s %s %s ", hp, color, univ);
		index = 0;
		c = fgetc(file);
		while(c != '\n'){
			nombre[index] = c;
			index++;
			c = fgetc(file);
		}
		nombre[index] = '\0';//para que finalize el string
		//Colocar atributos al guerrero
		guerreros[i].hp = atoi(hp);
		guerreros[i].colorUniverso = atoi(color);
		guerreros[i].numUniverso = atoi(univ);
		guerreros[i].nombre = nombre;
	}
	//Se libera memoria
	free(hp);
	free(color);
	free(univ);
	free(nombre);
	return guerreros;
}

//Funciones pantalla.
void createBoard(WINDOW *board, WINDOW *score){
	int max_x, max_y;
	int score_size = 6;

	getmaxyx(stdscr, max_y, max_x);
	board = newwin(max_y - score_size, max_x, 0, 0);
	score = newwin(score_size, max_x, max_y - score_size, 0);
	wborder(board, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(score, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(board, 0, max_x/2-1, "Board");
	mvwprintw(score, 0, max_x/2-1, "Score");
	wrefresh(board);
	wrefresh(score);
}

void deleteBoard(WINDOW *board, WINDOW *score){
	delwin(board);
	delwin(score);
}

void create_screen(){
	WINDOW *board, *score;

	initscr();
	noecho();
	curs_set(FALSE);
	refresh();
	createBoard(board, score);
	getch();//wait
	deleteBoard(board, score);
	endwin();



}
