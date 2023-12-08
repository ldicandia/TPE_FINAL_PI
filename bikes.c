#include "bikes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WEEKS 7
#define MAYOR(a,b) ((a)>(b)?(a):(b))

enum days_{DOM=0, LUN, MAR, MIE, JUE, VIE, SAB};


typedef struct oldest{ //query 2
    char * oldestDateTime;
    size_t oldestStationId;
}TOldest;

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

bikeADT string_cpy(bikeADT bike, char * from, size_t stationId){
    //char * temp = realloc(bike->station[stationId-1].nameStation, (strlen(from)+1) * sizeof(char));

    bike->station[stationId-1].nameStation = realloc(bike->station[stationId-1].nameStation, (strlen(from)+1) * sizeof(char));
    char * temp = malloc(strlen(from)+1);

    if (temp == NULL){
        return NULL;
    }

    bike->station[stationId-1].nameStation = temp;
    strcpy(bike->station[stationId-1].nameStation, from); 

    return bike;
}

static int _strcasecmp(const char *s1, const char *s2)
{
    while(*s1 && (toupper((unsigned char)*s1) == toupper((unsigned char)*s2)))
        s1++, s2++;
    return toupper((unsigned char)*s1) - toupper((unsigned char)*s2);
}

static int getDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int dayOfWeek = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    return dayOfWeek;
}

static size_t getDay(char *date ) {
    char *token = strtok(date, "-");
    int year = atoi(token);
    token = strtok(NULL, "-");
    int month = atoi(token);
    token = strtok(NULL, " ");
    int day = atoi(token);

    int dayOfWeek = getDayOfWeek(day, month, year);
    int days[WEEKS] = {DOM, LUN, MAR, MIE, JUE, VIE, SAB};
    
    return days[dayOfWeek];
}

static int compareInts(const void *a, const void *b) {
    int intA = *(const int *)a;
    int intB = *(const int *)b;
    return intA - intB;
}

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

static char * copyStr(const char * s){
    char * copy = malloc(strlen(s) + 1);
    if(copy == NULL) {
        return NULL; // or handle the error appropriately
    }
    return strcpy(copy, s);
}

/*-----------------------LOAD--------------------------------*/

bikeADT new(void){
    return calloc(1, sizeof(struct bikeCDT));
}

void putStation(bikeADT bike, char * startDate, size_t startId, char * endDate, size_t endId, size_t isMember, size_t yearFrom, size_t yearTo){
    size_t newSize = MAYOR(startId, endId);

    if (bike->resv_station < startId || bike->resv_station < endId){
        bike->station = realloc(bike->station, newSize * sizeof(TVecStation)); // Agrego memoria si es que el station dado es menor a resv_station

        for (int i = bike->resv_station; i < newSize; i++)
        {
            bike->station[i].memberTrips = 0;
            bike->station[i].casualTrips = 0;
            bike->station[i].allTrips = 0;
            bike->station[i].nameStation = NULL;
            bike->station[i].idStation = 0;
            bike->station[i].oldest.oldestStationId = 0;
            bike->station[i].oldest.oldestDateTime = NULL;
            for(int j = 0; j < WEEKS ; j++){
                bike->qtyPerDay[j].endedTrips = 0;
                bike->qtyPerDay[j].startedTrips = 0;
            }
        }
        bike->resv_station = newSize;
    }

//aumentamos cantidades depende si es miembro o no
    if(isMember){
        bike->station[startId-1].memberTrips++;
    }else{
        bike->station[startId-1].casualTrips++;
    }
    bike->station[startId-1].allTrips++;

    bike->qtyPerDay[getDay(startDate)].startedTrips++;  
    bike->qtyPerDay[getDay(endDate)].endedTrips++;

   
    if(bike->station[startId-1].oldest.oldestDateTime == NULL){
        bike->station[startId - 1].oldest.oldestDateTime = malloc(strlen(startDate)+1);
        bike->station[startId - 1].oldest.oldestDateTime = strcpy(bike->station[startId - 1].oldest.oldestDateTime, startDate);
    }
    else if (strcmp(startDate, bike->station[startId-1].oldest.oldestDateTime) < 0){
        bike->station[startId - 1].oldest.oldestDateTime = startDate;
        bike->station[startId - 1].oldest.oldestStationId = endId;
    }
}



//query 1

size_t getRealDim(bikeADT bike){
    return bike->dim_station;
}

size_t getMemberTrips(bikeADT bike, int pos){
    return bike->station[pos].memberTrips;
}

size_t getCausalTrips(bikeADT bike, int pos){
    return bike->station[pos].casualTrips;
}

size_t getAllTrips(bikeADT bike, int pos){
    return bike->station[pos].allTrips;
}

char * getStationName(bikeADT bike, int pos){
    return copyStr(bike->station[pos].nameStation);
}

void tripSort(bikeADT bike){
    int k=0;
    for (size_t i=0; i < bike->dim_station ; i++){
        if (bike->station[i].nameStation != NULL){
            free(bike->station[k].nameStation);  // liberar la cadena original
            bike->station[k].nameStation = copyStr(bike->station[i].nameStation);
            bike->station[k].memberTrips = bike->station[i].memberTrips;
            bike->station[k].allTrips = bike->station[i].allTrips;
            bike->station[k].casualTrips = bike->station[i].casualTrips;
            bike->station[k].oldest = bike->station[i].oldest;
            bike->station[k++].idStation = i+1;
            bike->station[k++].used = 0;
        }    
    }
    
    bike->station = realloc(bike->station, k * sizeof(TVecStation)); //Con este realloc eliminamos del vector todas las estaciones que tengan el used en 0.
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare);
}
/*query 2*/

/*query 3*/

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

// Ordena la matriz por orden alfabetico
void sortAlpha(bikeADT bike){

    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare_stationData);

}
/*----------------------FREES-----------------------------*/

void freeADT(bikeADT bike){ //terminar
    free(bike);
}















