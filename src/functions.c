#include "functions.h"

void call_getopt(int argc, char** argv, int *n, int *d, char **nombreArchivo){
	int c, hijos;
	char *Ivalue, *Nvalue, *Dvalue;
	

	//Obtener parametros.
	Dflag = 0;
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
	if(isInt(Nvalue)){
		printf("El parametro ingresado para el tamano de la tabla no es valido\n");
		exit(1);
	}

	if(isInt(Dvalue)){
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
      			return 1;
    		}
  	}

  	return 0;
}