
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

void putBikes(bikeADT bike, size_t startedId, size_t endedId, size_t * flagError);

void putStation(bikeADT bike, size_t startId, size_t isMember, char * returnDate, char * startDate,size_t endId, size_t yearFrom, size_t yearTo);

bikeADT string_cpy(bikeADT bike, char * from, size_t stationId);

void freeADT(bikeADT bike);

/*query 1*/

/*query 2*/ 

/*query 3*/

size_t startedTrips(bikeADT bike, int day);

size_t endedTrips(bikeADT bike, int day);

/*query 4*/


#endif
