
#ifndef __bikes_h_
#define __bikes_h_
#include <stdio.h>
#include <errno.h>

#define MAXCHAR 100

enum errors{OK=0, ARG_ERR, OPEN_ERR, CREA_ERR, MEMO_ERR, TOK_ERR};
/*
tipos de errores:
- ARG_ERR = error en la cantidad de argumentos
- OPEN_ERR = error al abrir un archivo
- CREA_ERR = error al crear el archivo
- MEMO_ERR = error de creacion de memoria
- TOK_ERR = error de token nulo
- NEX_ERR = error al chequear si hay un proximo en el iterador
*/

typedef struct bikeCDT * bikeADT;

bikeADT new(void);


/*query 1*/

/*query 2*/ 

/*query 3*/

size_t startedTrips(bikeADT bike, int day);

size_t endedTrips(bikeADT bike, int day);

/*query 4*/


#endif
