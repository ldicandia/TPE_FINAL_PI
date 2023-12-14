
#ifndef __bikes_h_
#define __bikes_h_
#include <stdio.h>
#include <errno.h>

#define MAXCHAR 200
#define MONTH 12
#define WEEKS 7
#define NAME_DISPLAY 3

/*
tipos de errores:
- ARG_ERR = error en la cantidad de argumentos
- OPEN_ERR = error al abrir un archivo
- CREA_ERR = error al crear el archivo
- MEMO_ERR = error de creacion de memoria
- TOK_ERR = error de token nulo
- POS_ERR = pos > dim.
- PAR_ERR = parsing liner error.
- ENT_ERR = error de entrada
- CPY_ERR = error de strcpy.
*/

enum errors{OK=0, ARG_ERR, OPEN_ERR, CREA_ERR, MEMO_ERR, TOK_ERR, POS_ERR, PAR_ERR, ENT_ERR, CPY_ERR};

typedef struct bikeCDT * bikeADT;

//Crea un nuevo ADT.

bikeADT new(void);

//Libera todos los recursos utilizados por el TAD.

void freeADT(bikeADT bike);

//devuelve el valor de la flag que indica tipo de errores detectados

size_t getErrorFlag(bikeADT bike);

//imprime el error segun corresponda.
void errorReturn(bikeADT bike);

// Agrega una bici al TAD.

void putBikes(bikeADT bike, size_t startedId, size_t endedId, size_t *flagError);

//Agrega una estacion al TAD.

void putStation(bikeADT bike, char * startDate, size_t startId, char * endDate, size_t endId, size_t isMember, size_t yearFrom, size_t yearTo);

//Copia el nombre en el ADT.

bikeADT string_cpy(bikeADT bike, char * from, size_t stationId);

//devuelve un duplicado del string

char * copyStr(const char * s);

/*query 1*/

//Devuelve el nombre de la estacioin en cierta posicion.

char * getStationName(bikeADT bike, size_t pos);

//Obtengo la dimensin real.

size_t getRealDim(bikeADT bike); 

//Obtengo el espacio reservado.

size_t getResv(bikeADT bike);

//devuelve 1 si la estacion esta siendo usada, 0 si no.

size_t getUsed(bikeADT bike, size_t idx);

//Devuelve la cantidad de miembros.

size_t getMemberTrips(bikeADT bike, size_t pos); 

//Devuelve la cantidad de viajeros casuales.

size_t getCausalTrips(bikeADT bike, size_t pos); 

//Devuelve la cantidad total de viajes sin importar si son o no miembros.

size_t getAllTrips(bikeADT bike, size_t pos);

//ordena el vector de estaciones por cantidad de viajes totales

void tripSort(bikeADT bike);

/*query 2*/ 

//Devuelve el id de la estacion con el viaje mas antiguo registrado.

size_t getOldestRoute(bikeADT bike, size_t pos);

//Devuelve la fecha en la que se registro el viaje mas antiguo.

char * getOldestDateTime(bikeADT bike, size_t pos);

//Devuelve el nombre de la estacion donde se registro el viaje mas antiguo.

char * getOldestName(bikeADT bike, size_t pos);

//Agrega el nombre al vector de Oldest.

void addNameToOldest(bikeADT bike, size_t pos);

/*query 3*/

//Devuelve la cantidad de viajes iniciados.

size_t getstartedTrips(bikeADT bike, int day, int * flag);

//Devuelve la cantidad de viajes finalizados.

size_t getEndedTrips(bikeADT bike, int day, int * flag);

//devuelve un dia de la semana.

char * getDayOfTheWeek(size_t day);

/*query 4*/

//Ordena en orden de MostPopular(ruta mas popular).

void sortMostPopularVec(bikeADT bike);

//Ordena en orden alfabetico.

void sortAlpha(bikeADT bike);

//Ordena en orden de mayor viajes circulares a menor.

void sortCircularVec(bikeADT bike);

//Devuelve el nombre del mas popular.

char *getMostPopRouteEndStation(bikeADT bike, size_t pos);

//Devuelve la cantidad de trips del mas popular.

size_t getMostPopRouteTrips(bikeADT bike, size_t pos);

//Agrega nombres al vector(solo los endStationTrips mas grandes).

void addNameToVec(bikeADT bike, size_t startId);

//Agrega al vector los endId.

void addVec(bikeADT bike, size_t startId, size_t endId);

/*query 5*/

//Agrega los viajes circulares al vector.

void addVecq5(bikeADT bike, size_t endId, size_t startId, char *startDate, char *endDate);

//Alegrega los nombres al vector de viaje circulares.

void addNameToVecQ5(bikeADT bike, size_t pos);

//Devuelve el nombre del vector circularStations de un determinado mes, en una determinada posicion.

char * getCircularName(bikeADT bike, size_t month, size_t pos);

//Devuelve el numero de un mes del anio.

size_t getMonth(char *startDate, char *endDate);

//Devuelve un mes del anio.

char * getMonthOfTheYear(size_t month);

//Devuele la dimension del TOP3 ya que peude estar el caso de que la dim sea menor.

size_t getDimMonthStations(bikeADT bike, size_t pos);

#endif

