#include "bikes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WEEKS 7

enum days_{DOM=0, LUN, MAR, MIE, JUE, VIE, SAB};

typedef struct rent{
    char * bikeType;
    char * bikeStart;
    char * bikeEnd;
    size_t idStart;
    size_t idEnd;
}TVecRent;

typedef struct oldest{
    char * oldestDateTime;
    size_t oldestStationId;
}TOldest;

typedef struct vecStation{
    TVecRent * bikes;
    size_t resv_stationBikes;
    size_t resvBikes;
    size_t idStation;
    char * nameStation;
    //query 1
    size_t memberTrips;
    size_t casualTrips;
    size_t allTrips;
     //query 2
    TOldest oldest;
    char * mostPopRouteEndStation;
    struct node * tail;
}TVecStation;


typedef struct q3{
    size_t startedTrips;
    size_t endedTrips;
}TQuery3;


typedef struct bikeCDT{
    TVecStation * station;       // vector ordenado por Id
    size_t resv_station;
    size_t dim_station;

    TQuery3 qtyPerDay[WEEKS];    //vector cant por dia;
    
    //query 4                    
    size_t ** matrix;            //matriz cuadrada de cantEstaciones, donde guardamos viaje entre cada estacion
    size_t qtyStations;          //resv_station matriz de adyacencia,      
}bikeCDT;

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
    token = strtok(NULL, "-");
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


void putStation(bikeADT bike, size_t startId, size_t isMember, char * returnDate, char * startDate,size_t endId, size_t yearFrom, size_t yearTo){
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
            bike->station[i].bikes = NULL;
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
    bike->qtyPerDay[getDay(returnDate)].endedTrips++;

   
    if(bike->station[startId-1].oldest.oldestDateTime == NULL){
        bike->station[startId - 1].oldest.oldestDateTime = malloc(strlen(startDate)+1);
        bike->station[startId - 1].oldest.oldestDateTime = strcpy(bike->station[startId - 1].oldest.oldestDateTime, startDate);
    }
    else if (strcmp(startDate, bike->station[startId-1].oldest.oldestDateTime) < 0){
        bike->station[startId - 1].oldest.oldestDateTime = startDate;
        bike->station[startId - 1].oldest.oldestStationId = endId;
    }
}

/*query 1*/

/*query 2*/

/*query 3*/

/*query 4*/








