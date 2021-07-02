#ifndef LIBRERIA_H
#define LIBRERIA_H

/*---------------------------------------------------------------------------*/
/* Declaración de constantes                                                 */
/*---------------------------------------------------------------------------*/

#define TAM_INICIAL 1
#define FACTOR 2

#define TAM_LECTORES 1

#define TAM_PRESTAMOS 1

#define SUCCESS 0
#define ERR_INICIAL 1
#define NOT_FOUND 2

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/

struct book {
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
};

struct libreria {
   struct book *array;
   int size_libros;
   int capacidad_libros;
};

struct registro_lectores {
	struct lector *usuario;
	int size_lectores;
    int capacidad_lectores;
};

struct registro_prestamos {
	struct loan *prestamos;
	int size_prestamos;
    int capacidad_prestamos;
};

struct lector{
  char *nombre;
  int tam_nombre;
  char *correo;
  int tam_correo;
  long int dni;
  unsigned int id_usuario;
  int prestado_usuario;
};

struct loan{
	unsigned int id_usuario;
	unsigned int id_libro;
	unsigned int id_prestamo;
        time_t fecha_inicio;
        time_t fecha_final;
	float valoracionprestamo;
	int devuelto;
};

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/


struct libreria anado_libro(struct libreria tabla_libros, int id, char *titulo_par, char *autor_par, char *fecha_par, int numero_lectores, int numero_lecturas, float valoracion, int prestado, int *error,int tam_titulo,int tam_autor,int tam_fecha);
void borro_memoria_libros(struct libreria tabla_libros);


struct registro_lectores registrar_lector(struct registro_lectores tabla_lectores,long int dni, char *correo,char *nombre,int *error);
struct registro_lectores borro_usuario(struct registro_lectores tabla_lectores, char *correo, int *error);
void borro(struct registro_lectores tabla_lectores);


int comparo_usuario(struct registro_lectores tabla_lectores, unsigned int id_usuario);
int comparo_libro(struct libreria tabla_libros, unsigned int id_libro);
int comparo_prestamo(struct registro_prestamos tabla_prestamos, unsigned int id_prestamo);

struct registro_prestamos add_loan(struct libreria tabla_libros,struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_libro, int *error);
struct registro_prestamos return_loan(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_prestamo, float valoracion, int *error);
void borro_prestamos(struct registro_prestamos tabla_prestamos);


void info_id(struct libreria tabla_libros);
void info_valoracion(struct libreria tabla_libros);
void info_prestamos(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario);
void info_prestamos_fecha(struct libreria tabla_libros, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_libro);

int comparo_id(const void *p, const void *q);
int comparo_valoracion(const void *p, const void *q);

#endif
