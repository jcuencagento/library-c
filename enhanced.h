#ifndef ENHANCED_H
#define ENHANCED_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum comandos_principales{add, reg, dereg, loans, info, import, help, out};
enum loans_opciones{lend, ret, defl};
enum info_opciones{byid, avg, reader, allreaders, defi};

struct comando{
	enum comandos_principales princ;
	enum loans_opciones op1;
	enum info_opciones op2;
	int int1;
	int int2;
	float f1;
	char *string1;
	char *string2;
	char *string3;
};

void info_lectores(struct registro_lectores tabla_lectores,struct registro_prestamos tabla_prestamos);

void CargarLibros_file(struct libreria *tabla_libros, char *load_file);
void CargarLectores_file(struct registro_lectores *tabla_lectores, char *load_file);

void GuardarLibros_file(struct libreria *tabla_libros, char *save_file);
void GuardarLectores_file(struct registro_lectores *tabla_lectores, char *save_file);


struct comando pedir_comando(int *c_error);
struct comando elijo_princ(char *token);
struct comando elijo_secun(struct comando c1, char *token);
struct comando elijo_tercer(struct comando c1, char *token);
struct comando elijo_cuarto(struct comando c1, char *token);
struct comando elijo_ultimo(struct comando c1, char *token);

#endif
