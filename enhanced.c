#include "libreria.h"
#include "persistencia.h"
#include "enhanced.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
//---------------------METODOS CARGAR Y GUARDAR CON ARCHIVO POR ARGV-------------------------------------------//


void info_lectores(struct registro_lectores tabla_lectores,struct registro_prestamos tabla_prestamos){
	for(int i=0; i<tabla_lectores.size_lectores; i++){
		for(int j=0; j<tabla_prestamos.size_prestamos; j++){ 
			if(tabla_prestamos.prestamos[j].id_usuario==tabla_lectores.usuario[i].id_usuario){
		       	printf("%i %s %s %li ----> ", tabla_lectores.usuario[i].id_usuario, tabla_lectores.usuario[i].nombre, tabla_lectores.usuario[i].correo, tabla_lectores.usuario[i].dni);			       	
		       	printf("%s %s %f \n", ctime(&(tabla_prestamos.prestamos[j].fecha_inicio)), ctime(&(tabla_prestamos.prestamos[j].fecha_final)), tabla_prestamos.prestamos[j].valoracionprestamo);
			}
		}
	}	
}
			

void CargarLibros_file(struct libreria *tabla_libros, char *load_file){
	
	struct libreria *tabla = tabla_libros;
	int tam_titulo;
	char *titulo;
	int tam_autor;
	char *autor;
	int tam_fecha;
	char *fecha;
	unsigned int id_libro;
	int numero_lectores;
	int numero_lecturas;
	float valoracion;
	int prestado;
	size_t leidos;
	int error;
	FILE *ficherolibros;
	int contador=0;
	 
	ficherolibros = fopen(load_file ,"r");    
	if(ficherolibros!=NULL){                       //NULL no vale hay que ver que no se añada el libro 0 0 0 0 0 -1 tal
		while (!feof(ficherolibros)){
			
			leidos = fread(&tam_titulo, 4, 1, ficherolibros);
			if (leidos != 1){ }
			titulo = (char *) malloc (sizeof(char)*(tam_titulo));
			fread(titulo, 1, tam_titulo, ficherolibros);
			if(titulo == NULL){
				printf("Error en el titulo.\n");  //Hacemos free aqui?
				break;
			}
			
			leidos = fread(&tam_autor, 4, 1, ficherolibros);
			autor = (char *) malloc (sizeof(char)*(tam_autor));
			fread(autor, 1, tam_autor, ficherolibros);
			if(autor == NULL){
				printf("Error en el autor.\n");  //Hacemos free aqui?
				break;
			}
			
			leidos = fread(&tam_fecha, 4, 1, ficherolibros);
			fecha = (char *) malloc (sizeof(char)*(tam_fecha));
			fread(fecha, 1, tam_fecha, ficherolibros);
			if(fecha == NULL){
				printf("Error en la fecha.\n");  //Hacemos free aqui?
				break;
			}
			
			leidos = fread(&id_libro, 4, 1, ficherolibros);
			leidos = fread(&numero_lectores, 4, 1, ficherolibros);
			leidos = fread(&numero_lecturas, 4, 1, ficherolibros);
			leidos = fread(&valoracion, 4, 1, ficherolibros);
			leidos = fread(&prestado, 4, 1, ficherolibros);
			
			#ifdef DEBUG
				printf ("Leidos los datos del libro %s\n",titulo);
			#endif
			
			if((titulo!=NULL) && (id_libro!=0)){
				*tabla = anado_libroCargar(*tabla_libros,id_libro,titulo,autor,fecha,numero_lectores, numero_lecturas, valoracion, prestado,&error, tam_titulo, tam_autor, tam_fecha);
			}
			
			contador++;
		}
		
		free(titulo);
		free(autor);
		free(fecha);
		
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



void CargarLectores_file(struct registro_lectores *tabla_lectores, char *load_file){
	
	struct registro_lectores *tabla = tabla_lectores;
	char *nombre;
	int tam_nombre;
	int tam_correo;
	char *correo;
	long int dni;
	unsigned int id_usuario;
	int prestado_usuario;
	size_t leidos;
	//long offset=0;
	int error;
	FILE *ficherolectores;
	int contador=0;
	
	ficherolectores = fopen(load_file ,"r");    
	if(ficherolectores!=NULL){
		while (!feof(ficherolectores)){
			
			leidos = fread(&tam_nombre, 4, 1, ficherolectores);
			 if (leidos != 1){ }
			nombre = (char *) malloc (sizeof(char)*(tam_nombre));
			fread(nombre, 1, tam_nombre, ficherolectores);
			if(nombre == NULL){
				printf("Error en el nombre.\n");  
				break;
			}
			
			leidos = fread(&tam_correo, 4, 1, ficherolectores);
			correo = (char *) malloc (sizeof(char)*(tam_correo));
			fread(correo, 1, tam_correo, ficherolectores);
			if(correo == NULL){
				printf("Error en el correo.\n"); 
				break;
			}
			
			
			leidos = fread(&dni, 4, 1, ficherolectores);
			leidos = fread(&id_usuario, 4, 1, ficherolectores);
			leidos = fread(&prestado_usuario, 4, 1, ficherolectores);
			
			
			#ifdef DEBUG
				printf ("Leidos los datos del lector%s\n",nombre);
			#endif
			
			if(id_usuario!=0 || dni!=0){
			  	*tabla = registrar_lectorCargar(*tabla_lectores,dni,correo,nombre, &error);
			}
			contador++;
		}
		
		free(nombre);
		free(correo);
		
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




void GuardarLibros_file(struct libreria *tabla_libros, char *save_file){
	
	struct libreria *tabla = tabla_libros;
	int tam_titulo;
	int tam_autor;
	int tam_fecha;
	FILE *faux;
	size_t escritos;
	faux = fopen(save_file,"w");
	
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



void GuardarLectores_file(struct registro_lectores *tabla_lectores, char *save_file){
	
	struct registro_lectores *tabla = tabla_lectores;
	int tam_nombre;
	int tam_correo;
	FILE *faux;
	size_t escritos;
	faux = fopen(save_file,"w");
	
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



struct comando pedir_comando(int *c_error){
	*c_error=0;
	struct comando c1;
	c1.princ=-1;
	c1.op1=0;
	c1.op2=0;
	c1.int1=0;
	c1.string1=NULL;
	c1.string2=NULL;
	c1.string3=NULL;
	
	char *cadena=NULL;
	char *delim, *token;
	char *saveptr1=NULL;
	int num_tokens = 0;	 
	
	int error=0;
	cadena=get_string(&error); // Pedimos una linea entera. ej: add;Yerma;Lorca;1934;84235
	delim = ";";
	#ifdef DEBUG
		printf("Cadena leida %s para delim %s\n", cadena, delim);
	#endif
	if (error == -3){
			*c_error=-3;
			c1.princ=out;
			if(cadena)
				free(cadena);
		  return c1;
		}
	while(error==0){
		if (error == -3){
			*c_error=-3;
			c1.princ=out;
			if(cadena)
				free(cadena);
		  return c1;
		}
		
		while ((token = strtok_r(cadena, delim, &saveptr1))!=NULL){  //CORTAMOS CADENA
			if (token==NULL){
				break;
			}
			if (cadena){
				cadena = NULL;
			}
			#ifdef DEBUG
			printf("Token [%i] = %s\n",num_tokens,token);
			#endif
			num_tokens++;							//AQUI DAREMOS UN VALOR A CADA OPCION DE COMANDO. ej c1.princ=loans c1.op1=lend........
			if(num_tokens==1){
				c1 = elijo_princ(token);	
			}
			if(num_tokens==2){
				c1 = elijo_secun(c1, token);
			}
			if(num_tokens==3){
				c1 = elijo_tercer(c1, token);
			}
			if(num_tokens==4){
				c1 = elijo_cuarto(c1, token);
			}
			if(num_tokens==5){
				c1 = elijo_ultimo(c1, token);
			}
		}
		if(error!=0){
			*c_error=error;
			printf("Error en strtok.");
		}else{
			if(cadena)
				free(cadena);
			return c1;		//Bien guardado
		}
	}
	return c1;
}


struct comando elijo_princ(char *token){
	struct comando c1;
	c1.princ=-1;
	c1.op1=defl;
	c1.op2=defi;
	c1.string1=NULL;
	c1.string2=NULL;
	c1.string3=NULL;
	c1.int1=-1;
	c1.int2=-1;
	c1.f1=-1;
	
	if(strcmp(token,"add")==0){
		c1.princ=add;
	}else if(strcmp(token,"reg")==0){
		c1.princ=reg;
	}else if(strcmp(token,"dereg")==0){
		c1.princ=dereg;
	}else if(strcmp(token,"loans")==0){
		c1.princ=loans;
	}else if(strcmp(token,"info")==0){
		c1.princ=info;
	}else if(strcmp(token,"import")==0){
		c1.princ=import;
	}else if(strcmp(token,"help")==0){
		c1.princ=help;
	}else if(strcmp(token,"out")==0){
		c1.princ=out;
	}else{
		puts("No es una funcionalidad principal válida. Tenemos estas: add, reg, dereg, loans, info, import, help, out");
	}
	return c1;
}
	
struct comando elijo_secun(struct comando c1, char *token){

	if(c1.princ==add){
		c1.string1 = token;
	}else if(c1.princ==reg){
		c1.string1 = token;
	}else if(c1.princ==dereg){
		c1.string1 = token;
	}else if(c1.princ==loans){
		if(strcmp(token,"lend")==0){
			c1.op1=lend;
		}else if(strcmp(token,"ret")==0){
			c1.op1=ret;
		}
	}else if(c1.princ==info){
		if(strcmp(token,"byid")==0){
			c1.op2=byid;
		}else if(strcmp(token,"avg")==0){
			c1.op2=avg;
		}else if(strcmp(token,"reader")==0){
			c1.op2=reader;
		}else if(strcmp(token,"allreaders")==0){
			c1.op2=allreaders;
		}
	}else if(c1.princ==import){
		char *ptr=NULL; 
		int number;
		number=strtol(token,&ptr,10);
		//if(*ptr!='\0'){
               //printf("No es un numero. Necesitamos un número de usuario.\n"); 
               //return c1;				//Confiamos en que el usuario lo ponga bien, poniendo lo de if ptr!=0 me da violacion
         //}else{
			 c1.int1=number;
		 //}
		 free(ptr);
	}else if(c1.princ==help){
		#ifdef DEBUG
			puts("Con help no es válido otro argumento");
		#endif
	}else if(c1.princ==out){
		#ifdef DEBUG
			puts("Con out no es válido otro argumento");
		#endif
	}else{
		puts("No es una argumento secundario válido. Podrían ser un titulo en add, un nombre en reg, un correo en dereg o una opción en loans o info.");
	}
	return c1;
}	
	
struct comando elijo_tercer(struct comando c1, char *token){
	
	if(c1.princ==add){
		c1.string2 = token;
	}else if(c1.princ==reg){
		c1.string2 = token;
	}else if(c1.princ==dereg){
		#ifdef DEBUG
			puts("Con dereg no hacen falta más argumentos");
		#endif
	}else if(c1.princ==loans){
		if(c1.op1==lend){
			char *ptr=NULL; 
			int number;
			number=strtol(token,&ptr,10);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
			c1.int1=number;
		}else if(c1.op1==ret){
			char *ptr=NULL; 
			int number;
			number=strtol(token,&ptr,10);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
			c1.int1=number;
		}else{
			#ifdef DEBUG
				puts("No era  una opción válida para los prestamos.");
			#endif
		}
	}else if(c1.princ==info){
		if(c1.op2==byid){
			#ifdef DEBUG
				puts("Con byid no hacen falta más argumentos");
			#endif
		}else if(c1.op2==avg){
			#ifdef DEBUG
				puts("Con avg no hacen falta más argumentos");
			#endif
		}else if(c1.op2==reader){
			char *ptr=NULL; 
			int number;
			number=strtol(token,&ptr,10);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
			c1.int1=number;
		}else if(c1.op2==allreaders){
			#ifdef DEBUG
				puts("Con allreaders no hacen falta más argumentos");
			#endif
		}else{
			#ifdef DEBUG
				puts("No era una opión de info.");
			#endif
		}
	}else if(c1.princ==import){
		c1.string1=token;
	}else if(c1.princ==help){
		#ifdef DEBUG
			puts("Con help no es válido otro argumento");
		#endif
	}else if(c1.princ==out){
		#ifdef DEBUG
			puts("Con out no es válido otro argumento");
		#endif
	}else{
		puts("No es un tercer argumento válido. Podrían ser una fecha en add, un correo en reg, un usuario en loans o info...");
	}
	return c1;
	
}


struct comando elijo_cuarto(struct comando c1, char *token){
	
	if(c1.princ==add){
		c1.string3 = token;
	}else if(c1.princ==reg){
		char *ptr=NULL; 
		int number;
		number=strtol(token,&ptr,10);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
		c1.int1=number;
	}else if(c1.princ==dereg){
		#ifdef DEBUG
			puts("Con dereg no hacen falta más argumentos");
		#endif
	}else if(c1.princ==loans){
		if(c1.op1==lend){
			char *ptr=NULL; 
			int number;
			number=strtol(token,&ptr,10);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
			c1.int2=number;
		}else if(c1.op1==ret){
			char *ptr=NULL; 
			int number;
			number=strtol(token,&ptr,10);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
			c1.int2=number;
		}else{
			#ifdef DEBUG
				puts("No era una opción válida para los prestamos.");
			#endif
		}
	}else if(c1.princ==info){
		if(c1.op2==byid){
			#ifdef DEBUG
				puts("Con byid no hacen falta más argumentos");
			#endif
		}else if(c1.op2==avg){
			#ifdef DEBUG
				puts("Con avg no hacen falta más argumentos");
			#endif
		}else if(c1.op2==reader){
			char *ptr=NULL; 
			int number;
			number=strtol(token,&ptr,10);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
			c1.int2=number;
		}else if(c1.op2==allreaders){
			#ifdef DEBUG
				puts("Con allreaders no hacen falta más argumentos");
			#endif
		}else{
			#ifdef DEBUG
				puts("No era una opión de info.");
			#endif
		}
	}else if(c1.princ==import){
		#ifdef DEBUG
			puts("Con import no son necesarios mas argumentos");
		#endif
	}else if(c1.princ==help){
		#ifdef DEBUG
			puts("Con help no es válido otro argumento");
		#endif
	}else if(c1.princ==out){
		#ifdef DEBUG
			puts("Con out no es válido otro argumento");
		#endif
	}else{
		puts("No es un cuarto argumento válido. Podrían ser una fecha en add, un dni en reg, un libro en loans o info...");
	}
	return c1;
	
}

struct comando elijo_ultimo(struct comando c1, char *token){
	
	if(c1.princ==add){
		char *ptr=NULL; 
		int number;
		number=strtol(token,&ptr,10); //Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
		c1.int1=number;
	}else if(c1.princ==reg){
		#ifdef DEBUG
			puts("Con reg no hacen falta más argumentos");
		#endif
	}else if(c1.princ==dereg){
		#ifdef DEBUG
			puts("Con dereg no hacen falta más argumentos");
		#endif
	}else if(c1.princ==loans){   //Suponemos solo para el ret
		char *ptr=NULL; 
		int number;
		number=strtof(token,&ptr);	//Confiamos en que lo pongan bien, poniendo lo de if ptr!=0 me da violacion
		c1.f1=number;
	}else if(c1.princ==info){
		#ifdef DEBUG
			puts("Con info no hacen falta más argumentos");
		#endif
	}else if(c1.princ==import){
		#ifdef DEBUG
			puts("Con import no hacen falta más argumentos");
		#endif
	}else if(c1.princ==help){
		#ifdef DEBUG
			puts("Con help no hacen falta más argumentos");
		#endif
	}else if(c1.princ==out){
		#ifdef DEBUG
			puts("Con out no hacen falta más argumentos");
		#endif
	}else{
		puts("No es un quinto argumento válido. Solo podrían ser una id de libro en add.");
	}
	return c1;	
}
