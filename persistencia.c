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
//---------------------METODOS CARGAR Y GUARDAR-----------------------------------------------

struct stat st = {0};

void CargarLibros(struct libreria *tabla_libros){
	struct libreria *tabla = tabla_libros;
	int tam_titulo;
	int tam_autor;
	int tam_fecha;
	unsigned int id_libro;
	int numero_lectores;
	int numero_lecturas;
	float valoracion;
	int prestado;
	size_t leidos;
	int error=0;
	FILE *ficherolibros;
	int contador=0;
	 
	ficherolibros = fopen("libros.txt" ,"r");    
	if(ficherolibros!=NULL){                       
		while (!feof(ficherolibros)){
			
			leidos = fread(&tam_titulo, 4, 1, ficherolibros);
			if (leidos != 1){
				break;
			}
			char *titulo = (char *) malloc (sizeof(char)*(tam_titulo));
			leidos = fread(titulo, 1, tam_titulo, ficherolibros);
			if (leidos != tam_titulo){
				free(titulo);
				break;
			}
			
			leidos = fread(&tam_autor, 4, 1, ficherolibros);
			if (leidos != 1){
				break;
			}
			char *autor = (char *) malloc (sizeof(char)*(tam_autor));
			leidos = fread(autor, 1, tam_autor, ficherolibros);
			if (leidos != tam_autor){
				free(autor);
				free(titulo);
				break;
			}
			
			leidos = fread(&tam_fecha, 4, 1, ficherolibros);
			if (leidos != 1){
				break;
			}
			char *fecha = (char *) malloc (sizeof(char)*(tam_fecha));
			leidos = fread(fecha, 1, tam_fecha, ficherolibros);
			if (leidos != tam_fecha){ 
				free(titulo);
				free(autor);
				free(fecha);
				break;
			}
			
			leidos = fread(&id_libro, 4, 1, ficherolibros);
			if (leidos != 1){ 
				break;
			}
			leidos = fread(&numero_lectores, 4, 1, ficherolibros);
			if (leidos != 1){
				break;
			}
			leidos = fread(&numero_lecturas, 4, 1, ficherolibros);
			if (leidos != 1){ 
				break;
			}
			leidos = fread(&valoracion, 4, 1, ficherolibros);
			if (leidos != 1){
				break;
			}
			leidos = fread(&prestado, 4, 1, ficherolibros);
			if (leidos != 1){
				break;
			}
			
			#ifdef DEBUG
				printf ("Leidos los datos del libro %s\n",titulo);
			#endif
			
				//if((titulo!=NULL) && (id_libro!=0)){
				*tabla = anado_libroCargar(*tabla_libros,id_libro,titulo,autor,fecha,numero_lectores, numero_lecturas, valoracion, prestado,&error, tam_titulo, tam_autor,tam_fecha);
				//}
			
			contador++;
		
			free(titulo);
			free(autor);
			free(fecha);
		}
		
		tabla_libros = tabla;
		
		if (ferror(ficherolibros)!=0){
			printf("Ha ocurrido un error mientras se leía.\n");
		}
	}else{
		puts("Fichero vacío.");
	}
	if (fclose(ficherolibros)!=0) {
		printf("Error al cerrar el fichero.\n");
	}
}
	
//RESUMEN:cargar y guardar los lectores
void CargarLectores(struct registro_lectores *tabla_lectores){
	struct registro_lectores *tabla = tabla_lectores;
	int tam_nombre;
	int tam_correo;
	long int dni;
	unsigned int id_usuario;
	int prestado_usuario;
	size_t leidos;
	//long offset=0;
	int error;
	FILE *ficherolectores;
	int contador=0;
	
	ficherolectores = fopen("lectores.txt" ,"r");    
	if(ficherolectores!=NULL){
		while (!feof(ficherolectores)){
			
			leidos = fread(&tam_nombre, 4, 1, ficherolectores);
			if (leidos != 1){ 
				break;
			}
			char *nombre = (char *) malloc (sizeof(char)*(tam_nombre));
			leidos = fread(nombre, 1, tam_nombre, ficherolectores);
			if (leidos != tam_nombre){
				free(nombre); 
				break;
			}
			
			leidos = fread(&tam_correo, 4, 1, ficherolectores);
			if (leidos != 1){
				break;
			}
			char *correo = (char *) malloc (sizeof(char)*(tam_correo));
			leidos = fread(correo, 1, tam_correo, ficherolectores);
			if (leidos != tam_correo){ 
				free(correo);
				free(nombre);
				break;
			}
			
			
			leidos = fread(&dni, 4, 1, ficherolectores);
			if (leidos != 1){ 
				break;
			}
			leidos = fread(&id_usuario, 4, 1, ficherolectores);
			if (leidos != 1){
				break;
			}
			leidos = fread(&prestado_usuario, 4, 1, ficherolectores);
			if (leidos != 1){ 
				break;
			}
			
			
			#ifdef DEBUG
				printf ("Leidos los datos del lector%s\n",nombre);
			#endif
			
				//if(id_usuario!=0 || dni!=0){
			  	*tabla = registrar_lectorCargar(*tabla_lectores,dni,correo,nombre, &error);
				//}
			contador++;
			
			free(nombre);
			free(correo);
		}
		
		
		
		tabla_lectores = tabla;
		
		if (ferror(ficherolectores)!=0){
			printf("Ha ocurrido un error mientras se leía.\n\n");
		}
	}else{
		puts("Fichero vacío.");
	}
	if (fclose(ficherolectores)!=0) {
		printf("Error al cerrar el fichero.\n");
	}
}
//RESUMEN: cargar los préstamos
void CargarPrestamos(struct registro_prestamos *tabla_prestamos,struct registro_lectores *tabla_lectores,struct libreria *tabla_libros){
	struct registro_prestamos *tabla = tabla_prestamos;
	unsigned int id_usuario;
	unsigned int id_libro;
	unsigned int id_prestamo;
	char fecha_inicio[128];
	char fecha_final[128];
	float valoracionprestamo;
	int devuelto;
	size_t leidos;
	int error;
	FILE *ficheroprestamos;
	int contador=0;
	
	ficheroprestamos = fopen("prestamos.txt" ,"r");    
	if(ficheroprestamos!=NULL){                        
		while (!feof(ficheroprestamos)){
			leidos = fread(&id_usuario, 4, 1, ficheroprestamos);
			if(leidos!=1){
				break;
			}
			leidos = fread(&id_libro, 4, 1, ficheroprestamos);
			if(leidos!=1){
				break;
			}
			leidos = fread(&id_prestamo, 4, 1, ficheroprestamos);
			if(leidos!=1){
				break;
			}
			
			leidos = fread(&fecha_inicio, sizeof(time_t), 1, ficheroprestamos);
			if(leidos!=1){
				break;
			}
			leidos = fread(&fecha_final, sizeof(time_t),1, ficheroprestamos);
			if(leidos!=1){
				break;
			}
			
			leidos = fread(&valoracionprestamo, 4, 1, ficheroprestamos);
			if(leidos!=1){
				break;
			}
			leidos = fread(&devuelto, 4, 1, ficheroprestamos);
			if(leidos!=1){
				break;
			}
			
			#ifdef DEBUG
				printf ("Leidos los datos de los prestamos");
			#endif
			
				if(id_usuario>=0 || id_libro!=0){
				*tabla = add_loanCargar(*tabla_libros,*tabla_lectores,*tabla_prestamos, id_usuario, id_libro, fecha_inicio, &error);
				if(devuelto==0){
					*tabla = return_loanCargar(*tabla_libros, *tabla_lectores, *tabla_prestamos, id_usuario, id_prestamo, valoracionprestamo, fecha_final,&error);
				}
				contador++;
				}
		}
		
		tabla_prestamos = tabla;
		
		if (ferror(ficheroprestamos)!=0){
			printf("Ha ocurrido un error mientras se leía.\n");
		}
	}else{
		puts("Fichero vacío.");
	}
	if (fclose(ficheroprestamos)!=0) {
		printf("Error al cerrar el fichero.\n");
	}
}
	
//RESUMEN: método para guardar los libros y se compruba que no haya errores
 void GuardarLibros(struct libreria *tabla_libros){
	struct libreria *tabla = tabla_libros;
	int tam_titulo;
	int tam_autor;
	int tam_fecha;
	FILE *faux;
	size_t escritos;
	faux = fopen("libros.txt","w");
	
    for(int i=0; i<tabla->size_libros; i++){

		tam_titulo=strlen(tabla->array[i].titulo)+1;
		escritos = fwrite(&tam_titulo,4,1,faux);
		if (escritos != 1){
			printf("Error en el tamaño del titulo.\n"); 
			break;
		}
		escritos = fwrite((tabla->array[i].titulo),1,tam_titulo,faux);
		if (escritos != tam_titulo){
			printf("Error en el titulo.\n"); 
			break;
		}
		
		tam_autor=strlen(tabla->array[i].autor)+1;
		escritos = fwrite(&tam_autor,4,1,faux);
		if (escritos != 1){
			printf("Error en el tamaño del autor.\n"); 
			break;
		}
		escritos = fwrite((tabla->array[i].autor),1,tam_autor,faux);
		if (escritos != tam_autor){
			printf("Error en el autor.\n"); 
			break;
		}

		tam_fecha=strlen(tabla->array[i].fecha)+1;
		escritos = fwrite(&tam_fecha,4,1,faux);
		if (escritos != 1){
			printf("Error en el tamaño de la fecha.\n"); 
			break;
		}
		escritos = fwrite((tabla->array[i].fecha),1,tam_fecha,faux);
		if (escritos != tam_fecha){
			printf("Error en la fecha.\n"); 
			break;
		}
	        
		escritos = fwrite(&tabla->array[i].id_libro,4,1,faux);
		if (escritos != 1){
			printf("Error en el id.\n"); 
			break;
		}
		escritos = fwrite(&tabla->array[i].numero_lectores,4,1,faux);
		if (escritos != 1){
			printf("Error en el numero de lectores.\n"); 
			break;
		}		
		escritos = fwrite(&tabla->array[i].numero_lecturas,4,1,faux);
		if (escritos != 1){
			printf("Error en el numero de lecturas.\n"); 
			break;
		}		
		escritos = fwrite(&tabla->array[i].valoracion,4,1,faux);
		if (escritos != 1){
			printf("Error en la valoracion.\n"); 
			break;
		}
		escritos = fwrite(&tabla->array[i].prestado,4,1,faux);
		if (escritos != 1){
			printf("Error en el prestado.\n"); 
			break;
		}
	}

	if (fclose(faux)!=0) {
		printf("Error al cerrar el fichero.\n");
	}

}

//RESEUMEN: método para guardar los lectores y se comprueba que no haya errores
 void GuardarLectores(struct registro_lectores *tabla_lectores){
	struct registro_lectores *tabla = tabla_lectores;
	int tam_nombre;
	int tam_correo;
	size_t escritos;
	FILE *faux;
	faux = fopen("lectores.txt","w");
	
	for(int i=0; i<tabla->size_lectores; i++){
		tam_nombre=strlen(tabla->usuario[i].nombre)+1;
		escritos = fwrite(&tam_nombre,4,1,faux);
		if (escritos != 1){
			printf("Error en el tamaño del nombre.\n"); 
			break;
		}
		escritos = fwrite((tabla->usuario[i].nombre),1,tam_nombre,faux);
		if (escritos != tam_nombre){
			printf("Error en el nombre.\n"); 
			break;
		}
		
		tam_correo=strlen(tabla->usuario[i].correo)+1;
		escritos = fwrite(&tam_correo,4,1,faux);
		if (escritos != 1){
			printf("Error en el tamaño del correo.\n"); 
			break;
		}
		escritos = fwrite((tabla->usuario[i].correo),1,tam_correo,faux);
		if (escritos != tam_correo){
			printf("Error en el correo.\n"); 
			break;
		}
		
		escritos = fwrite(&tabla->usuario[i].dni,4,1,faux);
		if (escritos != 1){
			printf("Error en el dni.\n"); 
			break;
		}
		escritos = fwrite(&tabla->usuario[i].id_usuario,4,1,faux);
		if (escritos != 1){
			printf("Error en el id.\n"); 
			break;
		}
		escritos = fwrite(&tabla->usuario[i].prestado_usuario,4,1,faux);
		if (escritos != 1){
			printf("Error en el prestado.\n"); 
			break;
		}
	}
		
	if (fclose(faux)!=0) {
		printf("Error al cerrar el fichero.\n");
	}
}

//RESUMEN :para guardar los prestamos
void GuardarPrestamos(struct registro_prestamos *tabla_prestamos){
	struct registro_prestamos *tabla = tabla_prestamos;
	FILE *faux;
	size_t escritos;
	//struct tm tm;
	//char buf[255];

	faux = fopen("prestamos.txt","w");
	
    for(int i=0; i<tabla->size_prestamos; i++){
		
		escritos = fwrite(&tabla->prestamos[i].id_usuario,4,1,faux);
		if (escritos != 1){
			printf("Error en el id del usuario.\n"); 
			break;
		}   
		escritos = fwrite(&tabla->prestamos[i].id_libro,4,1,faux);
		if (escritos != 1){
			printf("Error en el id del libro.\n"); 
			break;
		}
		escritos = fwrite(&tabla->prestamos[i].id_prestamo,4,1,faux);
		if (escritos != 1){
			printf("Error en el id del prestamo.\n"); 
			break;
		}
		//strptime(&tabla->prestamos[i].fecha_inicio,"%Y-%m-%d %H:%M:%S", &tm);
		//strftime(buf, sizeof(buf), "%d %b %Y %H:%M", &tm);
		escritos = fwrite(&tabla->prestamos[i].fecha_inicio,sizeof(time_t),1,faux);
		if (escritos != 1){
			printf("Error en la fecha.\n"); 
			break;
		}
	        //strptime(&tabla->prestamos[i].fecha_final,"%Y-%m-%d %H:%M:%S", &tm);
		//strftime(buf, sizeof(buf), "%d %b %Y %H:%M", &tm);
		escritos = fwrite(&tabla->prestamos[i].fecha_final,sizeof(time_t),1,faux);
		if (escritos != 1){
			printf("Error en el fecha.\n"); 
			break;
		}
		escritos = fwrite(&tabla->prestamos[i].valoracionprestamo,4,1,faux);
		if (escritos != 1){
			printf("Error en la valoracion.\n"); 
			break;
		}
		escritos = fwrite(&tabla->prestamos[i].devuelto,4,1,faux);
		if (escritos != 1){
			printf("Error en el devuelto.\n"); 
			break;
		}
        
	}

	if (fclose(faux)!=0) {
		printf("Error al cerrar el fichero.\n");
		}
}


 struct libreria anado_libroCargar(struct libreria tabla_libros, int id, char *titulo, char *autor, char *fecha, int numero_lectores, int numero_lecturas, float valoracion, int prestado, int *error,int tam_titulo,int tam_autor,int tam_fecha){

   int existe=0;
   *error = SUCCESS;
 
  
   /*Compruebo si lo metido por teclado ya esta en la libreria*/

   int i;

   for(i=0;i<tabla_libros.size_libros;i++){
     if ((tabla_libros.array[i].id_libro==id) || ( (strcmp(tabla_libros.array[i].titulo,titulo) ==0) && (strcmp(tabla_libros.array[i].autor,autor) ==0) && (strcmp(tabla_libros.array[i].fecha,fecha) ==0))){
		
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
		tabla_libros.array[tabla_libros.size_libros].tam_titulo = strlen(titulo)+1;
		if(tabla_libros.array[tabla_libros.size_libros].titulo == NULL){
			*error = errno;
			return tabla_libros;
		}
		
		errno=0;
		tabla_libros.array[tabla_libros.size_libros].autor = strdup(autor);
		tabla_libros.array[tabla_libros.size_libros].tam_autor = strlen(autor)+1;
		if(tabla_libros.array[tabla_libros.size_libros].autor == NULL){
			*error = errno;
			return tabla_libros;
		}
		
		errno=0;
		tabla_libros.array[tabla_libros.size_libros].fecha = strdup(fecha);
		tabla_libros.array[tabla_libros.size_libros].tam_fecha = strlen(fecha)+1;
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
	   //	printf("Se ha añadido el libro %s con identificador %i\n", tabla_libros.array[tabla_libros.size_libros].titulo,  tabla_libros.array[tabla_libros.size_libros].id_libro);
	   #endif
	   tabla_libros.size_libros++;
	   return tabla_libros;
   }else{
     return tabla_libros;
   }
 }

 
struct registro_lectores registrar_lectorCargar(struct registro_lectores tabla_lectores, long int dni, char *correo,char *nombre,int *error){
  int esta=0;
  
  for (int i=0; i < tabla_lectores.size_lectores; i++){
    if ((strcmp(tabla_lectores.usuario[i].correo,correo) ==0)){  
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
  //printf("Se ha registrado a este lector ->\n Nombre:%s  Correo:%s  Dni:%li Id:%i\n",nombre,correo,dni,tabla_lectores.usuario[tabla_lectores.size_lectores].id_usuario);
  tabla_lectores.size_lectores++;
  
  return tabla_lectores;
}
  else{
    return tabla_lectores;
  }
}

struct registro_prestamos add_loanCargar(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_libro, char *fecha_inicio, int *error){
	
  int contador3=0;
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

		        
				
			}else{
			  //puts("El libro ya está prestado"); //No hacemos prestamo
				return tabla_prestamos;  
			}
		}
	}
	return tabla_prestamos;
 }
 
 
struct registro_prestamos return_loanCargar(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_prestamo, float valoracion, char *fecha_final, int *error){
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
	
	return tabla_prestamos;
	}
