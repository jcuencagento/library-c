
//Fichero: util.c
//RESUMEN: se encuentran las funciones de get_string y get_number, éstas sirven para leer números  por teclado y detectar los posibles error que pueda haber. El get_number tiene una llamada a get_string.
//En get_string se hace una llamada a getline y en get_number se hace una llamada a strtol
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "util.h"


/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/
/*
RESUMEN: la llamada de esta funciónse encuntra en get_number,se utiliza getline que se encarga de leer una linea intriducida por teclado,devuelve el número de caracteres leidos, incluido el carácter
 delimitador, pero sin incluir el \0. La función get_string devuelve una cadena


 */
 
char *get_string(int *error) {
	*error= 0;
	char *cadena=NULL;
	size_t pedido=0;
	ssize_t leidos;
	errno=0;
	
	leidos=getline(&cadena,&pedido,stdin);
	if(leidos==-1){
		if(errno!=0){
			puts(" ");
			printf("Error!!");
			puts(" ");
			perror(strerror(errno));
		}else{
			puts(" ");
			printf("Es CTRL+D\n");
			clearerr(stdin);  
             *error = ERR_CTRLD_INPUT;
	        free(cadena);
		}
		cadena=NULL;

    }
	else{
		cadena[strlen(cadena)-1]='\0';		//notese que recortamos aqui la cadena 
		*error=SUCCESS_INPUT;
    }

	return cadena;
}

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/

/*
RESUMEN: Se comprueba si al leer lo introducido por teclado tiene algún error,se utiliza strtol, convierte la parte inicial de la cadena en nptr  a un valor entero de tipo long de acuerdo con la base dada, 
esta función devuelve el resultado de la conversión
*/

long int get_number(int *error){

	char *ptr=NULL; 
	long number;
    char *cadena= get_string(error);
	
	errno=0;
	number=strtol(cadena,&ptr,10);

	if(errno!=0){
             perror(strerror(errno));//imprime error
       }else if(*ptr!='\0'){
               printf("Error. No era un numero.\n");
               free(cadena);
               return -1;
         }

    free(cadena);
    return number;
}
