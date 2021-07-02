# <img src="https://github.com/jcuencagento/library-c/blob/main/src/c-mini-logo.jpg" width="50" height="50"> LIBRARY WITH C LANGUAGE


Trabajo de Arquitectura de Sistemas en la Universidad Carlos III de Madrid con la profesora Iria Estévez.
Realizado por Javier Cuenca Y Lucía Cabanillas.

Directorio dedicado exclusivamente a depositar la entrega ./saucem_enhanced 
tal y como se explica en el enunciado 

Recuerda que el proyecto se corrige compilando con los comandos:

gcc -Wall -g -DDEBUG *.c -lm
gcc -Wall -g *.c -lm

Se puede ejecutar con los argumentos ./saucem_enhanced [--help] [-l logfilePrefix]  [-i file] [-o file] [--no-load] [--no-save]
Para más ayuda ejecuta al principio con ./saucem_enhanced --help

Con los argumentos -i o -o, hace falta crear 3 ficheros y pasarlos como argumentos, uno para libros, otro para lectores y otro para prestamos.

Para dar órdenes, separa con punto y coma sin espacios, por favor.
Ejemplo:
add;Juego de Tronos;RR Martin;1996;5894(id)
reg;Javier;Cuenca;49051758
loans;lend;0(usuario);5894(id)
loans;ret;0(usuario);0(prestamo);4(valoracion)
info;avg
...


Tenemos 120 bytes de perdidas por cada linea introducida dentro del programa, por lo tanto no somos capaces de encontrar donde o como hacer free
de ese getline que nos da los leaks.
