
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

void freeADT(bikeADT bike);

void putBikes(bikeADT bike, size_t startedId, size_t endedId, size_t * flagError);

void putStation(bikeADT bike, char * startDate, size_t startId, char * endDate, size_t endId, size_t isMember, size_t yearFrom, size_t yearTo);

bikeADT string_cpy(bikeADT bike, char * from, size_t stationId);

void freeADT(bikeADT bike);

/*query 1*/
char * getStationName(bikeADT bike, size_t pos);

size_t getRealDim(bikeADT bike); 

size_t getMemberTrips(bikeADT bike, size_t pos); 

size_t getCausalTrips(bikeADT bike, size_t pos); 

size_t getAllTrips(bikeADT bike, size_t pos);

void tripSort(bikeADT bike);

/*query 2*/ 

size_t getOldestRoute(bikeADT bike, size_t pos);

char * getOldestDateTime(bikeADT bike, size_t pos);

/*query 3*/

size_t getstartedTrips(bikeADT bike, int day, int * flag);

size_t getEndedTrips(bikeADT bike, int day, int * flag);

/*query 4*/

void addMatrix(bikeADT bike, size_t station1Id, size_t station2Id, size_t * flagError);

void sortAlpha(bikeADT bike);

char * getMostPopRouteEndStation(bikeADT bike, size_t pos);

size_t getMostPopRouteTrips(bikeADT bike, size_t pos);

void freeADT(bikeADT bike);

#endif

