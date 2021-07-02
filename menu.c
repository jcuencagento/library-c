//FICHERO: menu.c
//RESUMEN: programa que se muestra al usuario antes de seleccionar una opción

#include <stdio.h>

#include "menu.h"


// Imprime todo el menú que se muetra por pantalla de la librería


void menu(){

		puts(" ");
	printf("\t./saucem_library\n");
	printf("Elige una opción:\n");
	printf("1. Añade un libro\n");
	printf("2. Registra un lector\n");
	printf("3. Borrar un lector\n");
	printf("4. Maneja préstamos\n");	
	printf("5. Info.\n");
	printf("6. Ayuda\n");
	printf("7. Importar desde CSV.\n");
	printf("0. Exit\n");
	printf("Si CTRL+D es pulsado en este menú, el programa terminará controladamente.\n");
	printf("Introduce una opción: \n");
	puts(" ");

}

void print_help(){
	puts("Programa de gestión de una biblioteca.");					//HELP
	puts(" ");
	puts("Uso:");
	puts("./saucem_enhanced [--help] [-l logfilePrefix]  [-i file] [-o file] [--no-load] [--no-save]");
	puts("--help                                 imprime esta ayuda y termina");
	puts("-l logfilePrefix                       indica el prefijo de los archivos de bitácora o log ");
	puts("-i file_libros file_lectores           indica los archivos de dónde debe leer la colección al inicio ");
	puts("-o file_libros file_lectores           indica el archivos dónde debe guardar la colecciónal terminar la ejecución del programa  ");
	puts("--no-load                              la colección empieza vacía");
	puts("--no-save                              no se guarda la colección al terminar");
	puts(" ");
}
