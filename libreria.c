#include "libreria.h"
#include "persistencia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>


int contador3 = 0;

//////////////////////////////
//METODO PARA ADD BOOK////////
/////////////////////////////


//RESUMEN: función para añadir un libro en la librería, se van haciendo comprobaciones de si los lobros se encuentran ya en la libreria o no, si hay capacidad o si hay algún error

struct libreria anado_libro(struct libreria tabla_libros, int id, char *titulo, char *autor, char *fecha, int numero_lectores, int numero_lecturas, float valoracion, int prestado, int *error,int tam_titulo,int tam_autor,int tam_fecha){

   int existe=0;
   *error = SUCCESS;
    
  
   /*Compruebo si lo metido por teclado ya esta en la libreria*/

   int i;

   for(i=0;i<tabla_libros.size_libros;i++){
     if ((tabla_libros.array[i].id_libro==id) || ( (strcmp(tabla_libros.array[i].titulo,titulo) ==0) && (strcmp(tabla_libros.array[i].autor,autor) ==0) && (strcmp(tabla_libros.array[i].fecha,fecha) ==0))){
		
       printf("Libro seleccionado ya se encuentra en la libreria\n");
       #ifdef DEBUG
       printf("Se trata de %s de %s escrito en %s\n",tabla_libros.array[i].titulo, tabla_libros.array[i].autor, tabla_libros.array[i].fecha);
       #endif
       existe=1;
       i = tabla_libros.size_libros;
     }
   }

   /*Si no ha sido introducido en la libreria lo introduzco*/
   if(existe==0){
	   size_t pedir;
	   int capacidad_nueva = 0;
	   if(tabla_libros.size_libros == tabla_libros.capacidad_libros){ //bien porque ambas son 0 (al principio) o porque se ha rellenado toda la capacidad
		 if(tabla_libros.capacidad_libros == 0){
		   capacidad_nueva = TAM_INICIAL;
		   pedir = TAM_INICIAL *sizeof(struct book); //pedimos espacio para meter libros, por eso vemos el tamaño de struct book
		   tabla_libros.size_libros = 0;
		   tabla_libros.array = NULL;
		  
		 }else{
		   capacidad_nueva = FACTOR * tabla_libros.capacidad_libros;
		   pedir = FACTOR * tabla_libros.capacidad_libros * sizeof(struct book);
		 }

		 struct book *tmp;
		 tmp = (struct book *) realloc (tabla_libros.array, pedir); //Recibe un puntero y un size_t
		 if(tmp == NULL){
		   *error = ERR_INICIAL;
		   /*no hay el espacio que hemos pedido todo seguido, ya que puede haber otros programas del ordenador ocupando espacio entre medias*/
		   return tabla_libros; //Sale del metodo
		 }
		 else{
		   tabla_libros.array = tmp;
		 }

		 tabla_libros.capacidad_libros = capacidad_nueva;
	   }

		errno = 0;
		tabla_libros.array[tabla_libros.size_libros].titulo = strdup(titulo);
		tabla_libros.array[tabla_libros.size_libros].tam_titulo = strlen(titulo)-1;
		if(tabla_libros.array[tabla_libros.size_libros].titulo == NULL){
			*error = errno;
			return tabla_libros;
		}
		
		errno=0;
		tabla_libros.array[tabla_libros.size_libros].autor = strdup(autor);
		tabla_libros.array[tabla_libros.size_libros].tam_autor = strlen(autor)-1;
		if(tabla_libros.array[tabla_libros.size_libros].autor == NULL){
			*error = errno;
			return tabla_libros;
		}
		
		errno=0;
		tabla_libros.array[tabla_libros.size_libros].fecha = strdup(fecha);
		tabla_libros.array[tabla_libros.size_libros].tam_fecha = strlen(fecha)-1;
		if(tabla_libros.array[tabla_libros.size_libros].fecha == NULL){
			*error = errno;
			return tabla_libros;
	   }
	   
	   tabla_libros.array[tabla_libros.size_libros].id_libro = id;
	   tabla_libros.array[tabla_libros.size_libros].prestado = prestado;
	   tabla_libros.array[tabla_libros.size_libros].numero_lectores = numero_lectores;
	   tabla_libros.array[tabla_libros.size_libros].numero_lecturas = numero_lecturas;
	   tabla_libros.array[tabla_libros.size_libros].valoracion = valoracion;
	   #ifdef DEBUG
		printf("Se ha añadido el libro %s con identificador %i\n", tabla_libros.array[tabla_libros.size_libros].titulo,  tabla_libros.array[tabla_libros.size_libros].id_libro);
	   #endif
	   tabla_libros.size_libros++;
	   return tabla_libros;
   }else{
     return tabla_libros;
   }
 }

//RESUMEN: método para borrar la memoria

void borro_memoria_libros(struct libreria tabla_libros){
   int i;
  for(i=0;i<tabla_libros.size_libros;i++){
    if(tabla_libros.array[i].autor!=NULL){
     free(tabla_libros.array[i].autor);
    }
    if(tabla_libros.array[i].titulo!=NULL){
     free(tabla_libros.array[i].titulo);
    }
    if(tabla_libros.array[i].fecha!=NULL){
    free(tabla_libros.array[i].fecha);
    }
  }
  free(tabla_libros.array);
}
  
//////////////////////////////
//MÉTODO PARA REGISTER USER//
//////////////////////////////

//RESUMEN: se registra un nuevo lector y se hará una comprobación de si ya está registrado o no
struct registro_lectores registrar_lector(struct registro_lectores tabla_lectores, long int dni, char *correo,char *nombre,int *error){
  int esta=0;
  for (int i=0; i< tabla_lectores.size_lectores; i++){
    if ((strcmp(tabla_lectores.usuario[i].correo,correo) ==0)){
		printf("Ya hay un lector registrados con esos datos -> Nombre:%s  Correo:%s  Dni:%li Id:%i\n",tabla_lectores.usuario[i].nombre,tabla_lectores.usuario[i].correo,tabla_lectores.usuario[i].dni,tabla_lectores.usuario[i].id_usuario);
		esta=1;
    }
  }
  
  if(esta==0){
  
  *error = SUCCESS;
  size_t pido;
  int nueva;
  if (tabla_lectores.size_lectores == tabla_lectores.capacidad_lectores){
    
    if (tabla_lectores.capacidad_lectores == 0){
      nueva = TAM_LECTORES;
      pido = TAM_LECTORES *sizeof(struct lector); // primera vez
      tabla_lectores.size_lectores = 0;
      tabla_lectores.usuario = NULL;

      
    }else{
      nueva = FACTOR * tabla_lectores.capacidad_lectores;
      pido = FACTOR * tabla_lectores.capacidad_lectores *sizeof(struct lector); // siguientes
    }
    
    struct lector *tmp2;
    tmp2 = (struct lector *) realloc (tabla_lectores.usuario, pido);
    if (tmp2 == NULL){
      *error = ERR_INICIAL;
      return tabla_lectores;
    }else{
      tabla_lectores.usuario = tmp2;
    }
    
    // no hay errores
    tabla_lectores.capacidad_lectores = nueva;
  }
  
  errno = 0;
  tabla_lectores.usuario[tabla_lectores.size_lectores].nombre = strdup(nombre); // para cadenas
  if (tabla_lectores.usuario[tabla_lectores.size_lectores].nombre == NULL){
    *error = errno;
    return tabla_lectores;
  }
  errno=0;
  tabla_lectores.usuario[tabla_lectores.size_lectores].correo = strdup(correo);
  if (tabla_lectores.usuario[tabla_lectores.size_lectores].correo == NULL){
    *error = errno;
    return tabla_lectores;
  }
  tabla_lectores.usuario[tabla_lectores.size_lectores].dni = dni;
  tabla_lectores.usuario[tabla_lectores.size_lectores].prestado_usuario = 0;
  tabla_lectores.usuario[tabla_lectores.size_lectores].id_usuario = tabla_lectores.size_lectores;
  printf("Se ha registrado a este lector ->\n Nombre:%s  Correo:%s  Dni:%li Id:%i\n",nombre,correo,dni,tabla_lectores.usuario[tabla_lectores.size_lectores].id_usuario);
  tabla_lectores.size_lectores++;
  
  return tabla_lectores;
}
  else{
    return tabla_lectores;
  }
}



void borro(struct registro_lectores tabla_lectores){
	for (int i = 0; i< tabla_lectores.size_lectores; i++){
		if (tabla_lectores.usuario[i].nombre != NULL){
			free(tabla_lectores.usuario[i].nombre);
		}
		if (tabla_lectores.usuario[i].correo != NULL){
			free(tabla_lectores.usuario[i].correo);
		}
	}
  free(tabla_lectores.usuario); 
}

//////////////////////////////
//Métodos para DEREGISTER USER
//////////////////////////////


 struct registro_lectores borro_usuario(struct registro_lectores tabla_lectores, char *correo, int *error){  
   *error = SUCCESS;
   int indice=-1;
   for (int i=0; i< tabla_lectores.size_lectores; i++){
     if (strcmp(tabla_lectores.usuario[i].correo,correo) == 0 ){
      indice= i;
     }

   }
  
  if (indice == -1){
    *error = NOT_FOUND;
    printf("No se ha encontrado su correo\n");
  }
  else{
    free(tabla_lectores.usuario[indice].nombre); // libero el nombre
    free(tabla_lectores.usuario[indice].correo);
    tabla_lectores.usuario[indice].nombre=NULL;
    tabla_lectores.usuario[indice].correo=NULL;
    tabla_lectores.usuario[indice].dni=0;
    tabla_lectores.usuario[indice].id_usuario=0;
    tabla_lectores.usuario[indice].prestado_usuario=0;
    if (indice == (tabla_lectores.size_lectores-1)){
      tabla_lectores.size_lectores--;
      
    }
    else{
      // copio el último en el sitio del que  borro
      memcpy(&(tabla_lectores.usuario[indice]), &(tabla_lectores.usuario[tabla_lectores.size_lectores-1]), sizeof(struct lector));
      tabla_lectores.size_lectores--;
    }
    printf("Se ha borrado un usuario\n");
  }
  return tabla_lectores;
 }
 
 
 ////////////////////////////
 //Métodos para MANAGE LOAN//
 ////////////////////////////
 
 
 
 //Métodos para ADD LOAN//

//RESUMEN: comprueba si el id del usuario está registrado. Si lo encuentra devuelve 1, si no 0.
 int comparo_usuario(struct registro_lectores tabla_lectores, unsigned int id_usuario){
	 for(int i=0; i<tabla_lectores.size_lectores; i++){
		 if(tabla_lectores.usuario[i].id_usuario==id_usuario){
			return 1;
		}
	}
	return 0;  //Si no ha habido match, devolvemos 0 como si fuera false

 }
 
 
 //RESUMEN: comprueba si el id del libro que ha seleccionado se encuentra en la biblioteca. Si lo encuentra devuelve 1, si no 0.
 int comparo_libro(struct libreria tabla_libros, unsigned int id_libro){
	 for(int i=0; i<tabla_libros.size_libros; i++){
		 if(tabla_libros.array[i].id_libro==id_libro){
			return 1;
		}
	}
	return 0;  //Si no ha habido match, devolvemos 0 como si fuera false
 }
 
 
 //RESUMEN: método que comprueba si el libro se ha encontrado, comprueba si ya está prestado y si no es así lo añade a la lista de prestamos realizados.
 struct registro_prestamos add_loan(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_libro, int *error){

	*error = SUCCESS;
	size_t pido;
	int nueva;
	
	for(int i=0; i<tabla_libros.size_libros; i++){
		 if(tabla_libros.array[i].id_libro==id_libro){
			if(tabla_libros.array[i].prestado==-1){      //Si no está prestado hacemos el método, si ya lo está, no.
				
				if(tabla_prestamos.size_prestamos == tabla_prestamos.capacidad_prestamos){
					if(tabla_prestamos.capacidad_prestamos == 0){
						nueva = TAM_PRESTAMOS;
						pido = TAM_PRESTAMOS *sizeof(struct loan); // primera vez
						tabla_prestamos.size_prestamos = 0;
						tabla_prestamos.prestamos = NULL;
					}else{
						nueva = FACTOR * tabla_prestamos.capacidad_prestamos;
						pido = FACTOR * tabla_prestamos.capacidad_prestamos *sizeof(struct loan); // siguientes
					}
					struct loan *tmp3;
					tmp3 = (struct loan *) realloc (tabla_prestamos.prestamos, pido);
					if (tmp3 == NULL){
						*error = ERR_INICIAL;
						return tabla_prestamos;
					}else{
						tabla_prestamos.prestamos = tmp3;
					}
					tabla_prestamos.capacidad_prestamos = nueva; // no hay errores
				}
	
				errno = 0;
				tabla_prestamos.prestamos[tabla_prestamos.size_prestamos].id_usuario = id_usuario;
				tabla_prestamos.prestamos[tabla_prestamos.size_prestamos].id_libro = id_libro;			//Hacemos prestamo
				tabla_prestamos.prestamos[tabla_prestamos.size_prestamos].id_prestamo = contador3;
				tabla_prestamos.prestamos[tabla_prestamos.size_prestamos].fecha_inicio = time(NULL);
				tabla_prestamos.prestamos[tabla_prestamos.size_prestamos].devuelto = 1;
				contador3++;
				
				for(int i=0; i<tabla_libros.size_libros; i++){
					 if(tabla_libros.array[i].id_libro==id_libro){
						tabla_libros.array[i].prestado=id_usuario; //Cambiamos la variable del libro prestado al numero del usuario al que se lo prestamos
						tabla_libros.array[i].numero_lectores++;   //Un lector más
					}
				}
				for(int i=0; i<tabla_lectores.size_lectores; i++){
					 if(tabla_lectores.usuario[i].id_usuario==id_usuario){
						tabla_lectores.usuario[i].prestado_usuario++;   //Una lectura más del usuario
					}
				}

				printf("Préstamo n.%i del libro n.%i hecho a %s\n",tabla_prestamos.prestamos[tabla_prestamos.size_prestamos].id_prestamo, id_libro, ctime(&(tabla_prestamos.prestamos[tabla_prestamos.size_prestamos].fecha_inicio)));
				
			}else{
				puts("El libro ya está prestado"); //No hacemos prestamo
				return tabla_prestamos;  
			}
		}
	}
	return tabla_prestamos;
 }
 
 
 
 //Métodos para RETURN LOAN//

 //RESUMEN: comprueba si el id del libro que ha seleccionado se encuentra en la biblioteca. Si lo encuentra devuelve 1, si no 0.
int comparo_prestamo(struct registro_prestamos tabla_prestamos, unsigned int id_prestamo){
	for(int i=0; i<tabla_prestamos.size_prestamos; i++){
		if(tabla_prestamos.prestamos[i].id_prestamo==id_prestamo){
			return 1;
		}
	}
	return 0;  //Si no ha habido match, devolvemos 0 como si fuera false
}


//RESUMEN: método que se ejecuta desde el main en la opción 4.2. Una vez sabemos que el libro se encuentra en los préstamos, pregunta por una valoración y añade el libro.
struct registro_prestamos return_loan(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_prestamo, float valoracion, int *error){
	*error = SUCCESS;
			
	float nueva_valoracion=0.00;
    tabla_prestamos.prestamos[id_prestamo].valoracionprestamo=valoracion;
    tabla_prestamos.prestamos[id_prestamo].devuelto = 0;
    
    for(int i=0; i<tabla_libros.size_libros; i++){
		 if(tabla_libros.array[i].id_libro==tabla_prestamos.prestamos[id_prestamo].id_libro){
			tabla_libros.array[i].prestado=-1;   //Cambiamos la variable del libro prestado a -1(NO prestado)
			tabla_libros.array[i].numero_lecturas++; //Una lectura acabada más
			
			for(int j=0; j<tabla_prestamos.size_prestamos; j++){
				if(tabla_libros.array[i].id_libro==tabla_prestamos.prestamos[j].id_libro){
					nueva_valoracion=nueva_valoracion+tabla_prestamos.prestamos[j].valoracionprestamo;
				}
			}
			tabla_libros.array[i].valoracion = nueva_valoracion/tabla_libros.array[i].numero_lecturas;
		}
	}
	
	tabla_prestamos.prestamos[id_prestamo].fecha_final = time(NULL);		//Devolvemos prestamo
	
	printf("Préstamo n.%i con valoracion %3.2f devuelto a %s\n",id_prestamo, valoracion, ctime(&(tabla_prestamos.prestamos[id_prestamo].fecha_final)));
	
	return tabla_prestamos;
	}

//liberar memoria

void borro_prestamos(struct registro_prestamos tabla_prestamos){
	
	/*for (int i = 0; i < tabla_prestamos.size_prestamos; i++){
		if (tabla_prestamos.prestamos[i].fecha_inicio != NULL){
			free(tabla_prestamos.prestamos[i].fecha_inicio);
		}
		if (tabla_prestamos.prestamos[i].fecha_final != NULL){
			free(tabla_prestamos.prestamos[i].fecha_final);
		}
	}*/
	free(tabla_prestamos.prestamos);
}


 //////////////////////
 //Métodos para INFO//
 /////////////////////
 
 
  //RESUMEN:se imprime la información ordenada, se ordena con la función qsort
void info_id(struct libreria tabla_libros){
  	qsort(tabla_libros.array, tabla_libros.size_libros, sizeof(struct book), comparo_id); //COMPARAMOS POR ID

	for(int i=0; i<tabla_libros.size_libros; i++){ 
		printf("%i    %s    %s    %s    %i    %i    %3.2f    %i\n", tabla_libros.array[i].id_libro, tabla_libros.array[i].titulo,
		       tabla_libros.array[i].autor, tabla_libros.array[i].fecha, tabla_libros.array[i].numero_lectores, tabla_libros.array[i].numero_lecturas,
		       tabla_libros.array[i].valoracion, tabla_libros.array[i].prestado); 
	}		        

}

  
 
//RESUMEN: Se utiliza la función qsort para ordenar los libros y se imprimen
void info_valoracion(struct libreria tabla_libros){
	qsort(tabla_libros.array, tabla_libros.size_libros, sizeof(struct book), comparo_valoracion);  //ORDENAMOS POR VALORACION
	for(int i=0; i<tabla_libros.size_libros; i++){ 	
		printf("%d    %s    %s    %s    %d    %d    %3.2f    %i\n", tabla_libros.array[i].id_libro, tabla_libros.array[i].titulo, tabla_libros.array[i].autor,
		       tabla_libros.array[i].fecha, tabla_libros.array[i].numero_lectores, tabla_libros.array[i].numero_lecturas,
		       tabla_libros.array[i].valoracion, tabla_libros.array[i].prestado); 
	}
}



void info_prestamos(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario){
	int librete;
	for(int i=0; i<tabla_prestamos.size_prestamos; i++){
		if(tabla_prestamos.prestamos[i].id_usuario==id_usuario){
			librete=tabla_prestamos.prestamos[i].id_libro;				
			for(int j=0; j<tabla_libros.size_libros; j++){
		   	if(tabla_libros.array[j].id_libro==librete){
		          	printf("%d     %s          %s     %s        %d    %3.2f   \n", librete, tabla_libros.array[j].titulo, tabla_libros.array[j].autor,
				       tabla_libros.array[j].fecha, tabla_libros.array[j].numero_lecturas, tabla_libros.array[j].valoracion);
				}
			}
		}
	}
}



void info_prestamos_fecha(struct libreria tabla_libros, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_libro){

	
	for(int i=0; i<tabla_prestamos.size_prestamos; i++){
		if(tabla_prestamos.prestamos[i].id_usuario==id_usuario){
			if(tabla_prestamos.prestamos[i].id_libro==id_libro){
				for(int j=0; j<tabla_libros.size_libros; j++){
					if(tabla_libros.array[j].id_libro==id_libro){
						if(tabla_libros.array[j].prestado==-1){
						        
							
       	printf("%s   %s   %3.2f \n", ctime(&(tabla_prestamos.prestamos[i].fecha_inicio)), ctime(&(tabla_prestamos.prestamos[i].fecha_final)), tabla_prestamos.prestamos[i].valoracionprestamo);

						}
					}
				}	
			}
		}
	}
}



//RESUMEN: método que se usará en la función qsort para ordenarlo, devuelve un número entero que será la resta de amos id que se están comparando
int comparo_id(const void *p, const void *q){
	struct book *f = (struct book*) p;
	struct book *s = (struct book*) q;
        return (f->id_libro - s->id_libro);
}


//RESUMEN: método que se usará en la función qsort para ordenarlo, devolverá un número dependiendo de las valoraciones.
//si la valoración del primero es menor que la del segundo devolverá un número positivo, en este caso un 1, si la valoración del segundo es menor que la del primero devolverá un -1
int comparo_valoracion(const void *p, const void *q){
	struct book *f = (struct book*) p;
	struct book *s = (struct book*) q;
	
	if(f->valoracion < s->valoracion){
		return 1;
	}else if(f->valoracion > s->valoracion){
		return -1;
	}else{
		return 0;
	}
}

