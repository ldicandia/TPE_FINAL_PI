#include "bikes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define WEEKS 7
#define MAYOR(a,b) ((a)>(b)?(a):(b))

enum days_{DOM=0, LUN, MAR, MIE, JUE, VIE, SAB};


typedef struct oldest{ //query 2
    char * oldestDateTime;
    size_t oldestStationId;
}TOldest;

typedef struct popular{
    char * mostPopRouteEndStation;
    size_t mostPopRouteTrips;
}TMostPopular;

typedef struct vecStation{
    size_t idStation;
    char * nameStation;
    char used;
    //query 1
    size_t memberTrips;
    size_t casualTrips;
    size_t allTrips;
     //query 2
    TOldest oldest;
    //query 4
    TMostPopular most;
}TVecStation;

typedef struct q3{ //query3
    size_t startedTrips;
    size_t endedTrips;
}TQuery3;

typedef struct bikeCDT{
    TVecStation * station;       // vector ordenado por Id
    size_t resv_station;
    size_t dim_station;
    TQuery3 qtyPerDay[WEEKS];    //vector cant por dia;  
    //query4
    size_t ** mat;
    size_t dim_mat;  
    
}bikeCDT;

/*------------------aux functions---------------------*/

bikeADT string_cpy(bikeADT bike, char *from, size_t stationId) {
    // Validar entradas
    if (bike == NULL || from == NULL) {
        fprintf(stderr, "Error: Entrada nula.\n");
        return NULL;
    }

    // Redimensionar o asignar nueva memoria para el nombre de la estación
    char *newName = realloc(bike->station[stationId - 1].nameStation, strlen(from) + 1);
    if (newName == NULL) {
        fprintf(stderr, "Error: Fallo en realloc.\n");
        return NULL;
    }

    // Actualizar el puntero y copiar la nueva cadena
    bike->station[stationId - 1].nameStation = newName;
    strcpy(bike->station[stationId - 1].nameStation, from);

    return bike;
}


static int _strcasecmp(const char *s1, const char *s2)
{
    while(*s1 && (toupper((unsigned char)*s1) == toupper((unsigned char)*s2)))
        s1++, s2++;
    return toupper((unsigned char)*s1) - toupper((unsigned char)*s2);
}


static int getDay(const char *day) {
    struct tm tm;
    time_t t;

    if (strptime(day, "%Y-%m-%d", &tm) == NULL) {
        fprintf(stderr, "Invalid date format: %s\n", day);
        return -1; 
    }

    tm.tm_hour = 0;  
    tm.tm_min = 0;  
    tm.tm_sec = 0;  
    tm.tm_isdst = -1;  

    
    t = mktime(&tm);

    if (t == -1) {
        fprintf(stderr, "Error converting time\n");
        return -1;
    }

    
    return tm.tm_wday;
}


/*
static int compareInts(const void *a, const void *b) {
    int intA = *(const int *)a;
    int intB = *(const int *)b;
    return intA - intB;
}
*/


static int compare(const void *a, const void *b){
    TVecStation *station1 = (TVecStation *)a;
    TVecStation *station2 = (TVecStation *)b;
    int cmp = 0;
    //Me fijo si el primero tiene menor cantidad de viajes o si tienen igual cantidad y son distintos lexicograficamente.
    if (station1->allTrips < station2->allTrips){
        cmp = 1;
    } else if (station1->allTrips > station2->allTrips){
        cmp = -1;
    }
    if (!cmp)
        cmp = _strcasecmp(station1->nameStation, station2->nameStation);
    
    return cmp;
}

static int compare_stationData(const void *a, const void *b) {
    TVecStation *dataA = (TVecStation *)a;
    TVecStation *dataB = (TVecStation *)b;

    return _strcasecmp(dataA->nameStation, dataB->nameStation); // Si 'name' es el campo que deseas comparar.
}

static char * copyStr(const char * s) {
    if (s == NULL) {
        return NULL;
    }
    char * copy = malloc(strlen(s) + 1);
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation error in copyStr\n");
        exit(1); // Or handle the error as appropriate
    }
    strcpy(copy, s);
    return copy;
}


/*-----------------------LOAD--------------------------------*/

bikeADT new(void){
    return calloc(1, sizeof(bikeCDT));
}

void putStation(bikeADT bike, char startDate[], size_t startId, char endDate[], size_t endId, size_t isMember, size_t yearFrom, size_t yearTo){
    if (bike == NULL){
        return;
    }
    if (startId == 0 || endId == 0){
        return;
    }
    if (startId == endId){
        return;
    }
    if (yearFrom > yearTo){
        return;
    }
    
    if (bike->dim_station < MAYOR(startId, endId)){
        bike->station = realloc(bike->station, MAYOR(startId, endId) * sizeof(TVecStation));
        if (bike->station == NULL){
            return;
        }
        for (int i = bike->dim_station; i < MAYOR(startId, endId); i++){
            bike->station[i].memberTrips = 0;
            bike->station[i].casualTrips = 0;
            bike->station[i].allTrips = 0;
            bike->station[i].nameStation = NULL;
            bike->station[i].idStation = 0;
            bike->station[i].oldest.oldestStationId = 0;
            bike->station[i].oldest.oldestDateTime = NULL;
            bike->station[i].used = 0;
            bike->station[i].most.mostPopRouteEndStation = NULL;
            bike->station[i].most.mostPopRouteTrips = 0;
            for(int j = 0; j < WEEKS ; j++){
                bike->qtyPerDay[j].endedTrips = 0;
                bike->qtyPerDay[j].startedTrips = 0;
            }
        }
        bike->dim_station = MAYOR(startId, endId);
    }
    //size_t newSize = MAYOR(startId, endId);

    if (bike->station[startId-1].used == 0){
        bike->station[startId-1].nameStation = malloc(strlen(startDate)+1);
        bike->station[startId-1].nameStation = strcpy(bike->station[startId-1].nameStation, startDate);
        bike->station[startId-1].idStation = startId;
        bike->station[startId-1].used = 1;
    }

    bike->station[startId-1].used = 1;


//aumentamos cantidades depende si es miembro o no
    if(isMember){
        bike->station[startId-1].memberTrips++;
    }else{
        bike->station[startId-1].casualTrips++;
    }
    bike->station[startId-1].allTrips++;


    //DEBUGEAR ESTO, GETDAY RECIBE LA HORA EN VEZ DE LA FECHA 16:42:00 ?????
    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    //quiero que getDay me devuelva el dia de la semana en numero yo le paso una fecha con formato 2022-11-17 19:05:10 y me devuelve el dia de la semana

    char aux[11];
    strncpy(aux, startDate, 10);
    aux[10] = 0;



    size_t start = getDay(aux);

    char aux2[11];
    strncpy(aux2, endDate, 10);
    aux2[10] = 0;
    size_t end = getDay(aux2); 


    bike->qtyPerDay[start].startedTrips++;  
    bike->qtyPerDay[end].endedTrips++;

   
    if(bike->station[startId-1].oldest.oldestDateTime == NULL){
        bike->station[startId-1].oldest.oldestDateTime = malloc(strlen(startDate)+1);
        bike->station[startId-1].oldest.oldestDateTime = strcpy(bike->station[startId-1].oldest.oldestDateTime, startDate);
        bike->station[startId-1].oldest.oldestStationId = endId;
    }
    else if (strcmp(startDate, bike->station[startId-1].oldest.oldestDateTime) < 0){
        bike->station[startId-1].oldest.oldestDateTime = strcpy(bike->station[startId-1].oldest.oldestDateTime, startDate);
        bike->station[startId-1].oldest.oldestStationId = endId;
    }
}



//query 1

size_t getRealDim(bikeADT bike){
    return bike->dim_station;
}

size_t getMemberTrips(bikeADT bike, size_t pos){
    return bike->station[pos].memberTrips;
}

size_t getCausalTrips(bikeADT bike, size_t pos){
    return bike->station[pos].casualTrips;
}

size_t getAllTrips(bikeADT bike, size_t pos){
    return bike->station[pos].allTrips;
}

char * getStationName(bikeADT bike, size_t pos){
    return copyStr(bike->station[pos].nameStation);
}

void tripSort(bikeADT bike){
    int k=0;
    for (size_t i=0; i < bike->dim_station ; i++){
        if (bike->station[i].used == 1){
            free(bike->station[k].nameStation);  // liberar la cadena original
            bike->station[k].nameStation = copyStr(bike->station[i].nameStation);
            bike->station[k].memberTrips = bike->station[i].memberTrips;
            bike->station[k].allTrips = bike->station[i].allTrips;
            bike->station[k].casualTrips = bike->station[i].casualTrips;
            bike->station[k].oldest = bike->station[i].oldest;
            bike->station[k].most.mostPopRouteEndStation = copyStr(bike->station[i].most.mostPopRouteEndStation);
            bike->station[k].most.mostPopRouteTrips = bike->station[i].most.mostPopRouteTrips;
            bike->station[k++].idStation = i+1;
            bike->station[k++].used = 1;
        }    
        bike->station[i].used = 0;
    }
    
    bike->station = realloc(bike->station, k * sizeof(TVecStation)); //Con este realloc eliminamos del vector todas las estaciones que tengan el used en 0.
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare);
}

/*query 2*/

//para el nombre hacemos un for con el getStationName y getDim y desp usamos las dos funciones de abajo

 //devuelve idx de oldest id entonces desp lo buscamos con getStationName
size_t getOldestRoute(bikeADT bike, size_t pos){
    return bike->station[pos].oldest.oldestStationId;
}

char * getOldestDateTime(bikeADT bike, size_t pos){//devuelve el oldest Date time 
    return copyStr(bike->station[pos].oldest.oldestDateTime);
}


/*query 3*/

size_t getstartedTrips(bikeADT bike, int day, int * flag){
    if(day > WEEKS){
        *flag = 1;
        return 0;
    }
    return bike->qtyPerDay[day].startedTrips;
}

size_t getEndedTrips(bikeADT bike, int day, int * flag){
    if(day > WEEKS){
        *flag = 1;
        return 0;
    }
    return bike->qtyPerDay[day].endedTrips;
}

/*query 4*/
void addMatrix(bikeADT bike, size_t startId, size_t endId, size_t * flagError){ // Crea la matriz de adyacencia
    size_t size = MAYOR(startId, endId);
    if (bike->dim_mat < size){
        // Agrego memoria para las filas
        bike->mat = realloc(bike->mat, size * sizeof(size_t*));
        if (bike->mat == NULL) {
            (*flagError) = MEMO_ERR;
            return;
        }
        // Para cada fila agrego memoria a las columnas
        for (size_t i = 0; i < size; i++) {
            if (i >= bike->dim_mat) {
                bike->mat[i] = NULL;  // Asegúrate de que bike->mat[i] esté inicializado
            }
            bike->mat[i] = realloc(bike->mat[i], size * sizeof(size_t));
            if (bike->mat[i] == NULL) {
                (*flagError) = MEMO_ERR;
                return;
            }
        }
        // Inicializamos una parte de la matriz con 0
        for (size_t i = 0; i < bike->dim_mat; i++) {
            for (size_t j = bike->dim_mat; j < size; j++) {
                bike->mat[i][j] = 0;
            }
        }
        for (size_t i = bike->dim_mat; i < size; i++){
            for (size_t j = 0; j < size; j++){
                bike->mat[i][j] = 0;
            }
        }
        bike->dim_mat = size; // Actualizamos el tamano de la matriz
    }
    // Llenamos la matriz con los datos
    bike->mat[startId-1][endId-1]++;
}

char * getMostPopRouteEndStation(bikeADT bike, size_t pos){
    return copyStr(bike->station[pos].most.mostPopRouteEndStation);
}

size_t getMostPopRouteTrips(bikeADT bike, size_t pos){
    return bike->station[pos].most.mostPopRouteTrips;
}

// Ordena la matriz por orden alfabetico
void sortAlpha(bikeADT bike){
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare_stationData);
}
/*----------------------FREES-----------------------------*/

void freeADT(bikeADT bike){ 
    for (size_t i = 0; i < bike->dim_station; i++){
        free(bike->station[i].nameStation);
        free(bike->station[i].oldest.oldestDateTime);
        free(bike->station[i].most.mostPopRouteEndStation);
    }
    free(bike->station);
    for (size_t i = 0; i < bike->dim_mat; i++){
        free(bike->mat[i]);
    }
    free(bike->mat);
    free(bike);
}















