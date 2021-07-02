#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "util.h"
#include "menu.h"
#include "libreria.h"
#include "persistencia.h"
#include "enhanced.h"


int main(int argc, char **argv){
	int vacio=0; //Hace de binario para directamente no entrar en un metodo si no hay ningun libro/lector
    int vacio_lectores=0;
        
	struct libreria tabla_libros;
        tabla_libros.array = NULL;
	tabla_libros.size_libros = 0;
        tabla_libros.capacidad_libros = 0;
	struct registro_lectores tabla_lectores;
        tabla_lectores.usuario = NULL;
        tabla_lectores.size_lectores = 0;					//Inicializacion
        tabla_lectores.capacidad_lectores = 0;
	struct registro_prestamos tabla_prestamos;
		tabla_prestamos.prestamos = NULL;
		tabla_prestamos.size_prestamos = 0;
		tabla_prestamos.capacidad_prestamos = 0;
	int numero_lectores;
	int numero_lecturas;
	float valoracion;
	int tam_titulo;
	int tam_autor;
	int tam_fecha;
	int prestado; 
        //int num_usuario;
	//int num_libro;
	//int num_prestamo;
	int usercorrect;
	int librocorrect;
	int prestamocorrect;
	int salir = 0;
	
	
	/*FILE *fichero_csv;
	ssize_t bytes_leidos;  //Variables para importar CSV
	size_t numero_bytes;
	char *cadena;*/
 
	printf("HOME : %s\n", getenv("HOME")); //Imprime de donde viene la carpeta (/home/astt) donde crearemos carpeta
	
	struct stat st = {0};
	if (stat("/home/astt/.saucem_library", &st) == -1){   //Crea la carpeta donde guardaremos la informacion, si no esta creada.
		mkdir("/home/astt/.saucem_library", 0700);
	}
	
	if(argc>1){
		if((strcmp(argv[1], "--help")==0)||(strcmp(argv[1], "-h")==0)){
			print_help();
			return -1;;
		}

		int conflicto_i=0;
		for(int i=1; i<argc;i++){
			if(strcmp(argv[i], "-i")==0){   	//INPUT FILE
				for(int j=1; j<argc;j++){
					if(strcmp(argv[j], "--no-load")==0){
						conflicto_i=1;
						puts("Conflicto en argumentos."); //En caso de conflicto, prevalece el --no-load
					}
				}
				if(conflicto_i==0){
					if (((access(argv[i+1],R_OK) != 0) )||((access(argv[i+1],F_OK) == 0)&& (access(argv[i+1],W_OK) != 0))){
						printf("No hay permiso de %s\n",argv[1]); 
						return -1;
					}
					CargarLibros_file(&tabla_libros, argv[i+1]);
					if(tabla_libros.array!=NULL){
						#ifdef DEBUG
						puts("Librería cargada.\n");
						#endif
						vacio=1;
					}
					if (((access(argv[i+2],R_OK) != 0) )||((access(argv[i+2],F_OK) == 0)&& (access(argv[i+2],W_OK) != 0))){
						printf("No hay permiso de %s\n",argv[1]); 
						return -1;
					}
					CargarLectores_file(&tabla_lectores, argv[i+2]);
					if(tabla_lectores.usuario!=NULL){					//LOAD EN ARCHIVO EN ESPECIAL
						#ifdef DEBUG
						puts("Usuarios cargados.\n");
						#endif
						vacio_lectores=1;
					}
					//CargarPrestamos_file(&tabla_prestamos,&tabla_lectores,&tabla_libros, argv[i+3]);
					if(tabla_prestamos.prestamos!=NULL){
						#ifdef DEBUG
						puts("Prestamos cargados.\n");
						#endif
					}
				}
			}
		}
		
		int no_load=0;
		for(int i=1; i<argc;i++){
			if(strcmp(argv[i], "--no-load")==0){
				no_load=1;
			}
		}
		if(no_load==0){
			CargarLibros(&tabla_libros);
			if(tabla_libros.array!=NULL){
				#ifdef DEBUG
				puts("Librería cargada.\n");
				#endif
				vacio=1;
				
			}
			CargarLectores(&tabla_lectores);
			if(tabla_lectores.usuario!=NULL){					//LOAD O NO LOAD
				#ifdef DEBUG
				puts("Usuarios cargados.\n");
				#endif
				vacio_lectores=1;
				
			}
			//CargarPrestamos(&tabla_prestamos,&tabla_lectores,&tabla_libros);
			if(tabla_prestamos.prestamos!=NULL){
				#ifdef DEBUG
				puts("Prestamos cargados.\n");
				#endif
			}
		}
		
				
		for(int i=1; i<argc;i++){
			if(strcmp(argv[i], "-l")==0){
				puts("No disponemos de esa funcion actualmente. \nDisculpen las molestias."); //LO DE BITACORA QUE NO HEMOS HECHO
			}
		}
		
	}else{
		CargarLibros(&tabla_libros);
		if(tabla_libros.array!=NULL){
			#ifdef DEBUG
			puts("Librería cargada.\n");
			#endif
			vacio=1;
		}
		CargarLectores(&tabla_lectores);
		if(tabla_lectores.usuario!=NULL){					//Load siempre si no hay argumentos
			#ifdef DEBUG
			puts("Usuarios cargados.\n");
			#endif
			vacio_lectores=1;
		}
		//CargarPrestamos(&tabla_prestamos,&tabla_lectores,&tabla_libros);
		if(tabla_prestamos.prestamos!=NULL){
			#ifdef DEBUG
			puts("Prestamos cargados.\n");
			#endif
		}
	}
	int c_error = 0;
	int error = 0;
	while(salir==0){	
		menu();  //imprime el menu
		
		//AQUI IRA FUNCION PEDIR COMANDO
		struct comando c1;
			c1.op1=defl;
			c1.op2=defi;
			c1.string1=NULL;
			c1.string2=NULL;
			c1.string3=NULL;
			c1.int1=-1;
			c1.int2=-1;
			c1.f1=-1;
		c1 = pedir_comando(&c_error);
		 
        if(c_error==-3){
			salir = 1;
        }else  if (c_error==0){
			switch(c1.princ){

				case add:
					numero_lectores = 0;
					numero_lecturas = 0;
					valoracion = 0.000000;
					prestado = -1;
					tam_titulo = 0;
					tam_autor = 0;
					tam_fecha = 0; 
					puts(" ");
					printf("Añade un libro.\n");
					puts(" ");
					if((c1.string1 ==NULL) || (c1.string2==NULL) || (c1.string3==NULL)){
						printf("Faltan argumentos.");
					}else{
						tabla_libros=anado_libro(tabla_libros, c1.int1, c1.string1, c1.string2, c1.string3, numero_lectores, numero_lecturas, valoracion, prestado, &error, tam_titulo, tam_autor, tam_fecha);
					}
					vacio=1;
					puts(" ");
				break;

					case reg:
					puts(" "); 
					printf("\t Registra un lector.\n");
					puts(" ");
					if((c1.string1 ==NULL) || (c1.string2==NULL)){
						printf("Faltan argumentos.");
					}else{
						tabla_lectores=registrar_lector(tabla_lectores, c1.int1, c1.string2,c1.string1,&error);
					}
					vacio_lectores=1;
					puts(" ");
				break;

				case dereg:
					printf("\t Borra un lector.");
					puts(" ");
					
					borro_usuario(tabla_lectores, c1.string1, &error);
	
					puts(" ");
				break;
				
				
				case loans: 
					
					printf("\t Maneja préstamos.");
					puts(" ");
					if(vacio==0){
						puts("No hay libros en la colección.");      //Si no hemos entrado en Add Book, salimos directamente.
					}else{
						puts(" ");
						if((c1.op1==1) || (c1.op1==0)){
							switch(c1.op1){
								case lend:  //LOAN
									
									usercorrect = comparo_usuario(tabla_lectores, c1.int1);
									if(usercorrect==1){				//Si existe el usuario, le preguntamos el libro
										
										librocorrect = comparo_libro(tabla_libros, c1.int2);
										if(librocorrect==1){		//Si existe el libro, se lo prestamos.
											tabla_prestamos = add_loan(tabla_libros, tabla_lectores, tabla_prestamos, c1.int1, c1.int2, &error);
											tabla_prestamos.size_prestamos++;			
										}
										else{
											puts("\t No hemos encontrado ese libro.");
											break;
										}
									}else{
										puts("\t No hemos encontrado ese usuario.");
										break;
									}
								break;
									
								case ret: //RETURN
									usercorrect = comparo_usuario(tabla_lectores, c1.int1);
									if(usercorrect==1){	
										prestamocorrect = comparo_prestamo(tabla_prestamos, c1.int2);
										if(prestamocorrect==1){		//Si existe el prestamo, se lo pedimos.
											if(tabla_prestamos.prestamos[c1.int2].devuelto == 1){
												tabla_prestamos = return_loan(tabla_libros, tabla_lectores, tabla_prestamos, c1.int1, c1.int2, c1.f1, &error);
											}else{
												puts("Ya esta devuelto");
											}
										}else{
											puts("\t No hemos encontrado ese préstamo.");
											break;
										}
									}else{
										puts("\t No hemos encontrado ese usuario.");
										break;
									}		
								break;
								
								default:
									puts(" ");  
									printf("\t No es una opción.\n");
									puts(" ");
								break;
							}
						}else{
							printf("\t Escribe una opción válida: lend o ret.\n");
						}
					}		
					puts(" ");
				break;
				
				case info: 
					if((vacio==1)||(vacio_lectores==1)){
						if(error==0){
							if((c1.op2==1) || (c1.op2==0) || (c1.op2==2)|| (c1.op2==3)){
								switch(c1.op2){
									case byid:  //POR ID
										puts(" ");
										puts("Libros ordenados por Id:");
											puts("Id.   Título.   Autor.   Año de publicación.   Lectores.    Lecturas.   Valoración.   Prestado.");
										info_id(tabla_libros);  //Imprimimos por id.

									break;
									
									case avg: //POR VALORACION
										puts(" ");
										puts("Libros ordenados por valoración:");
										puts("Id.   Título.   Autor.   Año de publicación.   Lectores.    Lecturas.   Valoración.   Prestado.");
										info_valoracion(tabla_libros);  //Imprimimos por valoracion.


									break;
									
									case reader:  //POR LECTOR
									
										usercorrect = comparo_usuario(tabla_lectores, c1.int1);
										if(usercorrect==1){	
											librocorrect = comparo_libro(tabla_libros, c1.int2);
											if(librocorrect==1){		//Si existe el libro, enseñamos todas las lecturas de este usuario.
												puts("Fecha de inicio.    \tFecha de fin.     \tValoración.");
													info_prestamos_fecha(tabla_libros, tabla_prestamos, c1.int1, c1.int2); //Imprimimos fechas de prestamos del libro 
											}else{
												puts("\t No hemos encontrado ese libro.");
												break;
											}
										}else{
											puts("\t No hemos encontrado ese usuario.");
											break;
										}		
									break;
									
									case allreaders:
									  puts(" ");
									  if(vacio_lectores==0){
										puts("No hay lectores para poder decir sus prestamos");
									  }else{
										puts("Lector.                 Préstamos.");
										info_lectores(tabla_lectores,tabla_prestamos);
									  }
									  break;
									  
									default:
										puts(" "); 
										#ifdef DEBUG
											printf("Error en tipo de info.\n");
										#endif
										puts(" ");
									break;
								}
							}else{
							printf("\t Escribe una opción válida: byid, avg, reader o allreaders.\n");
							}
						}else{
							printf("\t Error.\n");
						}
					}else{
						puts("No hay nada que mostrar.");
					}
				break;
				
				case help:
					puts(" ");
					printf("\t Ayuda.");  //HELP
					puts(" ");
				break;
				
				case import:
					puts("\t No tenemos esta función disponible.");
					/*
					usercorrect = comparo_usuario(tabla_lectores, c1.int1);		//IMPORTAR DESDE CSV
					if(usercorrect==1){	
						 while (strcmp(".csv",c1.string1+strlen(c1.string1)-4)!=0){  //Comprobamos que es .csv
							printf("Error! No es un fichero .csv\n");
							break;
						}
						numero_bytes = 0;
						cadena = NULL;
						fichero_csv = fopen(c1.string1, "rb");
						bytes_leidos = getline(&cadena, &numero_bytes, fichero_csv);
						if (bytes_leidos == -1){
							puts("Error.");
						}else{
							puts("El fichero es:");
							printf(cadena);
						}
						free(cadena);	
					}else{
						puts("No existe ese usuario:");
					}*/
				break;
				
				case out:
					salir=1;
				break;
				
				default:  
				break;
			}
		
		}else{
			printf("No entra al switch.\n");
		}
	}
	
	if(argc>1){
		
		int conflicto_o=0;
		for(int i=1; i<argc;i++){
			if(strcmp(argv[i], "-o")==0){   	//OUTPUT FILE
				for(int j=1; j<argc;j++){
					if(strcmp(argv[j], "--no-save")==0){   //En caso de conflicto, predomina --no-save
						conflicto_o=1;
						puts("Conflicto en argumentos.");
					}
				}
				if(conflicto_o==0){
					if(tabla_libros.size_libros!=0){
						if (((access(argv[i+1],R_OK) != 0) )||((access(argv[i+1],F_OK) == 0)&& (access(argv[i+1],W_OK) != 0))){
							printf("No hay permiso de %s\n",argv[1]); 
							return -1;
						}
						GuardarLibros_file(&tabla_libros, argv[i+1]);
					}	
					if(tabla_lectores.size_lectores!=0){
						if (((access(argv[i+2],R_OK) != 0) )||((access(argv[i+2],F_OK) == 0)&& (access(argv[i+2],W_OK) != 0))){
							printf("No hay permiso de %s\n",argv[1]); 
							return -1;
						}
						GuardarLectores_file(&tabla_lectores, argv[i+2]);	//LOAD EN ARCHIVO EN ESPECIAL
					}
					if(tabla_prestamos.size_prestamos!=0){
						//GuardarPrestamos_file(&tabla_prestamos,&tabla_lectores,&tabla_libros, argv[i+3]);
					}
				}
			}
		}
		
		int no_save = 0;
		for(int i=1; i<argc;i++){
			if(strcmp(argv[i], "--no-save")==0){
				no_save = 1;
			}
		}
		if(no_save == 0){
			if(tabla_libros.size_libros!=0){
				GuardarLibros(&tabla_libros);
			}
			if(tabla_lectores.size_lectores!=0){
				GuardarLectores(&tabla_lectores);   //Save o no save.
			}
			if(tabla_prestamos.size_prestamos!=0){
				//GuardarPrestamos(&tabla_prestamos);
			}
		}
	}else{
		if(tabla_libros.size_libros!=0){
			GuardarLibros(&tabla_libros);
		}
		if(tabla_lectores.size_lectores!=0){
			GuardarLectores(&tabla_lectores);   //Save si no hay argumentos.
		}
		if(tabla_prestamos.size_prestamos!=0){
		  //GuardarPrestamos(&tabla_prestamos);
		}
	}
	
    borro_memoria_libros(tabla_libros);
    borro(tabla_lectores);
    borro_prestamos(tabla_prestamos);

  return 0;
}
