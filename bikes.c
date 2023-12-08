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









