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
    size_t mostPopRouteEndStationId;
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

    // Actualizar el puntero y copiar la nueva cadena
    bike->station[stationId - 1].nameStation = realloc(bike->station[stationId - 1].nameStation, strlen(from)+1);
    char * aux = strcpy(bike->station[stationId-1].nameStation, from);
    //free(aux); 

    return bike;
}


static int _strcasecmp(const char *s1, const char *s2)
{
    while(*s1 && (toupper((unsigned char)*s1) == toupper((unsigned char)*s2)))
        s1++, s2++;
    return toupper((unsigned char)*s1) - toupper((unsigned char)*s2);
}


static int getDay(const char *dateString) {
    struct tm dateStruct;
    memset(&dateStruct, 0, sizeof(struct tm));

    sscanf(dateString, "%d-%d-%d", &dateStruct.tm_year, &dateStruct.tm_mon, &dateStruct.tm_mday);
    
    dateStruct.tm_year -= 1900;  
    dateStruct.tm_mon -= 1;      

    time_t t = mktime(&dateStruct);

    struct tm *localTimeStruct = localtime(&t);

    return localTimeStruct->tm_wday;
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

    return _strcasecmp(dataA->nameStation, dataB->nameStation); 
}

static char * copyStr(const char * s) {
    if(s == NULL){
        fprintf(stderr, "Falla cpy");
        exit(1);
    }
    char * res = malloc(strlen(s)+1);
    if (res == NULL) 
        return NULL;
    
    return strcpy(res, s);
}

/*-----------------------LOAD--------------------------------*/

bikeADT new(void){
    return calloc(1, sizeof(bikeCDT));
}

void putStation(bikeADT bike, char startDate[], size_t startId, char endDate[], size_t endId, size_t isMember, size_t yearFrom, size_t yearTo){
    //if (bike == NULL || (startId == 0 || endId == 0) || (startId == endId) || (yearFrom > yearTo)){
    //    fprintf(stderr, "Error parameters");
    //    exit(1);
    //}
    
    if (bike->resv_station < MAYOR(startId, endId)){
        bike->station = realloc(bike->station, MAYOR(startId, endId) * sizeof(TVecStation));
        for (int i = bike->resv_station; i < MAYOR(startId, endId); i++){
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
        bike->resv_station = MAYOR(startId, endId);
    }
    //size_t newSize = MAYOR(startId, endId);

    if (bike->station[startId-1].used == 0){
        bike->station[startId-1].idStation = startId;
        bike->station[startId-1].used = 1;
        bike->dim_station++;
    }

    if(isMember){
        bike->station[startId-1].memberTrips++;
    }else{
        bike->station[startId-1].casualTrips++;
    }
    bike->station[startId-1].allTrips++;

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
    if(bike->station[pos].nameStation == NULL){//cambiar por exit(1)
        return NULL;
    }
    
    if(bike->dim_station < pos){
        fprintf(stderr, "pos > dim");
        exit(1);
    }

    if(bike->station[pos].nameStation == NULL){
        fprintf(stderr, "Passing NULL bike getStationName");
        exit(1);
    }

    return copyStr(bike->station[pos].nameStation);
}

void tripSort(bikeADT bike){
    int k=0;
    for (size_t i=0; i < bike->resv_station ; i++){
        if (bike->station[i].used == 1){
            free(bike->station[k].nameStation);  // liberar la cadena original
            if(bike->station[i].nameStation != NULL){
                bike->station[k].nameStation = copyStr(bike->station[i].nameStation);
                //free(bike->station[i].nameStation);
            }
            bike->station[k].memberTrips = bike->station[i].memberTrips;
            bike->station[k].allTrips = bike->station[i].allTrips;
            bike->station[k].casualTrips = bike->station[i].casualTrips;
            bike->station[k].oldest = bike->station[i].oldest;
            
            if(bike->station[i].most.mostPopRouteEndStation != NULL){
                bike->station[k].most.mostPopRouteEndStation = copyStr(bike->station[i].most.mostPopRouteEndStation);
                free(bike->station[i].most.mostPopRouteEndStation);
            }
            //agregarIdMost
            bike->station[k].most.mostPopRouteTrips = bike->station[i].most.mostPopRouteTrips;
            bike->station[k].idStation = i+1;
            bike->station[k++].used = 1;
        }
        bike->station[i].used = 0;
    }
    
    bike->station = realloc(bike->station, k*sizeof(TVecStation)); //Con este realloc eliminamos del vector todas las estaciones que tengan el used en 0.
    bike->dim_station = bike->resv_station = k;
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare);
}

/*query 2*/

//para el nombre hacemos un for con el getStationName y getDim y desp usamos las dos funciones de abajo

 //devuelve idx de oldest id entonces desp lo buscamos con getStationName
size_t getOldestRoute(bikeADT bike, size_t pos){
    return bike->station[pos].oldest.oldestStationId;
}

char * getOldestDateTime(bikeADT bike, size_t pos){//devuelve el oldest Date time 
    if(bike->dim_station < pos){
        fprintf(stderr, "pos > dim");
        exit(1);
    }

    if(bike->station[pos].nameStation == NULL){
        fprintf(stderr, "Passing NULL bike getOldestDateTime");
        exit(1);
    }
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

char * getDayOfTheWeek(size_t day){
    const char * days[] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
    return days[day];
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
    /*
    bike->mat[startId-1][endId-1]++;
    if(bike->mat[startId-1][endId-1] > bike->station[startId-1].most.mostPopRouteTrips){
        bike->station[startId-1].most.mostPopRouteTrips = bike->mat[startId-1][endId-1];
        
        if(bike->station[endId-1].nameStation != NULL){
            bike->station[startId-1].most.mostPopRouteEndStation = malloc(strlen(bike->station[endId-1].nameStation)+1); 
            strcpy(bike->station[startId-1].most.mostPopRouteEndStation, bike->station[endId-1].nameStation);
        }
    }
    */
    // Incremento del contador de viajes para esta ruta
    bike->mat[startId-1][endId-1]++;
    
    // Verificar si esta ruta es ahora la mas popular
    if (bike->mat[startId-1][endId-1] > bike->station[startId-1].most.mostPopRouteTrips) {
        bike->station[startId-1].most.mostPopRouteTrips = bike->mat[startId-1][endId-1];
        bike->station[startId-1].most.mostPopRouteEndStationId = endId;
    }
}

void addMost(bikeADT bike){
    size_t aux = 0;
    for(int i = 1 ; i < bike->resv_station ; i++){
        if(bike->station[i-1].used){
            aux = bike->station[i-1].most.mostPopRouteEndStationId;
            //bike->station[i-1].most.mostPopRouteEndStation = copyStr(bike->station[aux-1].nameStation); 
            bike->station[i-1].most.mostPopRouteEndStation = malloc(strlen(bike->station[aux-1].nameStation)+1);
            strcpy(bike->station[i-1].most.mostPopRouteEndStation, bike->station[aux-1].nameStation);
        }
    }
}


char * getMostPopRouteEndStation(bikeADT bike, size_t pos){
    if(bike->station[pos].most.mostPopRouteEndStation == NULL){ //cambiar por exit 1
        return NULL;
    }
    
    if(bike->dim_station < pos){
        fprintf(stderr, "pos > dim");
        exit(1);
    }

    if(bike->station[pos].nameStation == NULL){
        fprintf(stderr, "Passing NULL bike getMostPopular");
        exit(1);
    }
    return copyStr(bike->station[pos].most.mostPopRouteEndStation);
}

size_t getMostPopRouteTrips(bikeADT bike, size_t pos){
    return bike->station[pos].most.mostPopRouteTrips;
}

// Ordena el vector por orden alfabetico
void sortAlpha(bikeADT bike){
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare_stationData);
}

/*----------------------FREES-----------------------------*/

void freeADT(bikeADT bike){ //libera toda la memoria
    if(bike == NULL){
        exit(1);
    }
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

