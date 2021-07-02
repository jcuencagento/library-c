#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/* Declaración de constantes                                                 */
/*---------------------------------------------------------------------------*/



#define TAMANHO 100


/*---------------------------------------------------------------------------*/
/* Prototipos de las funciones                                               */
/*---------------------------------------------------------------------------*/

void CargarLibros(struct libreria *tabla_libros);
void CargarLectores(struct registro_lectores *tabla_lectores);
void CargarPrestamos(struct registro_prestamos *tabla_prestamos,struct registro_lectores *tabla_lectores,struct libreria *tabla_libros);

void GuardarLibros(struct libreria *tabla_libros);
void GuardarLectores(struct registro_lectores *tabla_lectores);
void GuardarPrestamos(struct registro_prestamos *tabla_prestamos);


struct libreria anado_libroCargar(struct libreria tabla_libros, int id, char *titulo, char *autor, char *fecha, int numero_lectores, int numero_lecturas, float valoracion, int prestado, int *error,int tam_titulo,int tam_autor,int tam_fecha);
struct registro_lectores registrar_lectorCargar(struct registro_lectores tabla_lectores, long int dni, char *correo,char *nombre,int *error);
struct registro_prestamos add_loanCargar(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_libro, char *fecha_inicio, int *error);
struct registro_prestamos return_loanCargar(struct libreria tabla_libros, struct registro_lectores tabla_lectores, struct registro_prestamos tabla_prestamos, unsigned int id_usuario, unsigned int id_prestamo, float valoracion, char *fecha_final, int *error);
#endif
