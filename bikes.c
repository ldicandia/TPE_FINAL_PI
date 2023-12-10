#include "bikes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_DATE 10
#define WEEKS 7
#define MAYOR(a,b) ((a)>(b)?(a):(b))

enum days_{DOM=0, LUN, MAR, MIE, JUE, VIE, SAB};


typedef struct oldest{ 
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
    
    size_t memberTrips;
    size_t casualTrips;
    size_t allTrips;
    
    TOldest oldest;
    
    TMostPopular most;
}TVecStation;

typedef struct q3{ 
    size_t startedTrips;
    size_t endedTrips;
}TQuery3;

typedef struct bikeCDT{
    TVecStation * station;       
    size_t resv_station;
    size_t dim_station;
    TQuery3 qtyPerDay[WEEKS];    
    
    size_t ** mat;
    size_t dim_mat;  
    
}bikeCDT;

/*------------------aux functions---------------------*/

bikeADT string_cpy(bikeADT bike, char *from, size_t stationId) {
    if (bike == NULL || from == NULL) {
        fprintf(stderr, "Error: Entrada nula.\n");
        return NULL;
    }
    free(bike->station[stationId-1].nameStation);
    bike->station[stationId-1].nameStation = realloc(bike->station[stationId-1].nameStation, strlen(from)+1);
    strcpy(bike->station[stationId-1].nameStation, from);
    

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

static int compare(const void *a, const void *b){
    TVecStation *station1 = (TVecStation *)a;
    TVecStation *station2 = (TVecStation *)b;
    int cmp = 0;
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
        exit(MEMO_ERR);
    }
    char * res = malloc(strlen(s)+1);
    if (res == NULL) 
        return NULL;

    res = strcpy(res, s);
    
    return res;
}

/*-----------------------LOAD--------------------------------*/

bikeADT new(void){
    return calloc(1, sizeof(bikeCDT));
}

void putStation(bikeADT bike, char startDate[], size_t startId, char endDate[], size_t endId, size_t isMember, size_t yearFrom, size_t yearTo){
    
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

    char aux[MAX_DATE + 1];
    strncpy(aux, startDate, MAX_DATE);
    aux[MAX_DATE] = 0;
    size_t start = getDay(aux);

    char aux2[11];
    strncpy(aux2, endDate, MAX_DATE);
    aux2[MAX_DATE] = 0;
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
    if(bike->station[pos].nameStation == NULL){
        return NULL;
    }
    
    if(bike->dim_station < pos){
        fprintf(stderr, "pos > dim");
        exit(POS_ERR);
    }

    if(bike->station[pos].nameStation == NULL){
        fprintf(stderr, "Passing NULL bike getStationName");
        exit(POS_ERR);
    }

    return copyStr(bike->station[pos].nameStation);
}

void tripSort(bikeADT bike){
    int k=0;
    for (size_t i=0; i < bike->resv_station ; i++){
        if (bike->station[i].used == 1){
            free(bike->station[k].nameStation);  
            if(bike->station[i].nameStation != NULL){
                bike->station[k].nameStation = copyStr(bike->station[i].nameStation);
                
            }
            bike->station[k].memberTrips = bike->station[i].memberTrips;
            bike->station[k].allTrips = bike->station[i].allTrips;
            bike->station[k].casualTrips = bike->station[i].casualTrips;
            bike->station[k].oldest = bike->station[i].oldest;
            
            
            if(bike->station[i].most.mostPopRouteEndStation != NULL){
                bike->station[k].most.mostPopRouteEndStation = copyStr(bike->station[i].most.mostPopRouteEndStation);
            }
            bike->station[k].most.mostPopRouteTrips = bike->station[i].most.mostPopRouteTrips;
            bike->station[k].idStation = i+1;
            bike->station[k++].used = 1;
        }
        bike->station[i].used = 0;
    }
    
    bike->station = realloc(bike->station, k*sizeof(TVecStation)); 
    bike->dim_station = bike->resv_station = k;
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare);
}

/*query 2*/

size_t getOldestRoute(bikeADT bike, size_t pos){
    return bike->station[pos].oldest.oldestStationId;
}

char * getOldestDateTime(bikeADT bike, size_t pos){
    if(bike->dim_station < pos){
        fprintf(stderr, "pos > dim");
        exit(POS_ERR);
    }

    if(bike->station[pos].nameStation == NULL){
        fprintf(stderr, "Passing NULL bike getOldestDateTime");
        exit(TOK_ERR);
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
    char * days[] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
    return days[day];
}

/*query 4*/

void addMatrix(bikeADT bike, size_t startId, size_t endId, size_t * flagError){ 
    size_t size = MAYOR(startId, endId);
    if (bike->dim_mat < size){
        
        bike->mat = realloc(bike->mat, size * sizeof(size_t*));
        if (bike->mat == NULL) {
            (*flagError) = MEMO_ERR;
            return;
        }
     
        for (size_t i = 0; i < size; i++) {
            if (i >= bike->dim_mat) {
                bike->mat[i] = NULL;  
            }
            bike->mat[i] = realloc(bike->mat[i], size * sizeof(size_t));
            if (bike->mat[i] == NULL) {
                (*flagError) = MEMO_ERR;
                return;
            }
        }
        
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
        bike->dim_mat = size; 
    }
    
    bike->mat[startId-1][endId-1]++;
    
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
            bike->station[i-1].most.mostPopRouteEndStation = copyStr(bike->station[aux-1].nameStation);
        }
    }
}

char * getMostPopRouteEndStation(bikeADT bike, size_t pos){
    if(bike->station[pos].most.mostPopRouteEndStation == NULL){ 
        return NULL;
    }
    
    if(bike->dim_station < pos){
        fprintf(stderr, "pos > dim");
        exit(POS_ERR);
    }

    if(bike->station[pos].nameStation == NULL){
        fprintf(stderr, "Passing NULL bike getMostPopular");
        exit(TOK_ERR);
    }
    return copyStr(bike->station[pos].most.mostPopRouteEndStation);
}

size_t getMostPopRouteTrips(bikeADT bike, size_t pos){
    return bike->station[pos].most.mostPopRouteTrips;
}

void sortAlpha(bikeADT bike){
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare_stationData);
}

/*----------------------FREES-----------------------------*/

void freeADT(bikeADT bike){ //libera toda la memoria
    if(bike == NULL){
        exit(MEMO_ERR);
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
