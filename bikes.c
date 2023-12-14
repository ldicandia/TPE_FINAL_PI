#include "bikes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define NAME_DISPLAY 3
#define MAX_DATE 10
#define BLOCK 20
#define WEEKS 7
#define MAYOR(a,b) ((a)>(b)?(a):(b))
#define MENOR(a, b) ((a) < (b) ? (a) : (b))
#define DATE_SIZE 16

//enum months_{JAN = 0, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC}; 
//enum days_{LUN = 0, MAR, MIE, JUE, VIE, SAB, DOM};

 //query 2
typedef struct oldest{ 
    char * oldestDateTime;
    char * oldestEndStation;
    size_t oldestStationId;
}TOldest;

//query 4//

typedef struct vecpopular{
    char * endStation;
    size_t endStationId; 
    size_t endStationTrips;
}TVecPopular;


typedef struct vecStation{
    size_t idStation;
    char * nameStation;
    size_t used;
    
    //query 1
    size_t memberTrips;
    size_t casualTrips;
    size_t allTrips;
    
    //query 2
    TOldest oldest;
    
    //query 4

    TVecPopular * most_vec;
    size_t dim_most; 
    size_t resv_most;
    
    //query5 
    size_t circularTrips; 
}TVecStation;


//query 3
typedef struct q3{ 
    size_t startedTrips;
    size_t endedTrips;
}TQuery3;

//query 5

typedef struct stations{
    char * nameStation;
    size_t circularTrips; 
    size_t startId; 
}stations; 

typedef struct q5{
    stations * circularStations; 
    size_t resStat; 
    size_t dimStat; 
}TQuery5; 

typedef struct bikeCDT{
    TVecStation * station;       
    size_t resv_station;
    size_t dim_station;
    size_t errorFlag;

    //query3
    TQuery3 qtyPerDay[WEEKS];  
    TQuery5 month[MONTH]; 
}bikeCDT;

/*------------------aux functions---------------------*/

size_t getErrorFlag(bikeADT bike){
    return bike->errorFlag;
}

bikeADT string_cpy(bikeADT bike, char *from, size_t stationId) {
    if (bike == NULL || from == NULL) {
        
        bike->errorFlag= ENT_ERR;
    }
    
    bike->station[stationId-1].nameStation = malloc(strlen(from)+1);
    strcpy(bike->station[stationId-1].nameStation, from);
    

    return bike;
    
}

static int _strcasecmp(const char *s1, const char *s2){
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

    return localTimeStruct->tm_wday; //CAMBIO -1
}


// si hay un viaje circular se llama a getmonth
// getmonth devuelve el numero del mes donde ocurrio el viaje circular
// si el viaje circular empieza en un mes y termina en otro, devuelve -1


size_t getMonth(char *startDate, char *endDate){
    // el formato de las fechas es 2021-09-20 06:31:28 y solo quiero tener el mes, en este caso 09

    size_t month1;
    size_t month2;

    char *token = strtok(startDate, "-"); // salteo la primera parte hasta llegar al mes
    token = strtok(NULL, "-");            // copio mes
    month1 = strtoul(token, NULL, 10);

    token = strtok(endDate, "-"); // salteo la primera parte hasta llegar al mes
    token = strtok(NULL, "-");    // copio mes
    month2 = strtoul(token, NULL, 10);

    if (month1 == month2)
    {
        return month1-1;
    }

    return -1;
}


static int compare(const void *a, const void *b)
{
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

char * copyStr(const char * s) {
    if(s == NULL){
        return NULL; 
    }
    char * res = malloc(strlen(s)+1);
    if (res == NULL) 
        return NULL;

    strcpy(res, s);
    
    return res;
}

static int compare_circular(const void *a, const void *b){
    stations *station1 = (stations *)a;
    stations *station2 = (stations *)b;
    int cmp = 0;
    if (station1->circularTrips < station2->circularTrips){
        cmp = 1;
    } else if (station1->circularTrips > station2->circularTrips){
        cmp = -1;
    }    
    return cmp;
}

/*-----------------------LOAD--------------------------------*/

bikeADT new(void){
    bikeADT new = calloc(1, sizeof(bikeCDT));
    if(new == NULL){
        new->errorFlag = MEMO_ERR;
    }
    return new;
}

void putStation(bikeADT bike, char startDate[], size_t startId, char endDate[], size_t endId, size_t isMember, size_t yearFrom, size_t yearTo){
    //agregamos espacio si se lo necesita
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
            bike->station[i].oldest.oldestEndStation = NULL;
            bike->station[i].used = 0;

            bike->station[i].most_vec = NULL;
            bike->station[i].dim_most = 0;
            bike->station[i].resv_most = 0;

            
            for(int j = 0; j < WEEKS ; j++){
                bike->qtyPerDay[j].endedTrips = 0;
                bike->qtyPerDay[j].startedTrips = 0;
            }
        }
        bike->resv_station = MAYOR(startId, endId);
    }

    //si no estaa usado, ponemos el id
    if (bike->station[startId-1].used == 0){
        bike->station[startId-1].idStation = startId;
        bike->station[startId-1].used = 1;
        bike->dim_station++;
    }

    //query 1
    if(isMember){
        bike->station[startId-1].memberTrips++;
    }else{
        bike->station[startId-1].casualTrips++;
    }
    bike->station[startId-1].allTrips++;

    //query 3
    char aux[MAX_DATE + 1];
    strncpy(aux, startDate, MAX_DATE);
    aux[MAX_DATE] = 0;
    size_t start = getDay(aux);

    char aux2[MAX_DATE + 1];
    strncpy(aux2, endDate, MAX_DATE);
    aux2[MAX_DATE] = 0;
    size_t end = getDay(aux2); 

    bike->qtyPerDay[start].startedTrips++;  
    bike->qtyPerDay[end].endedTrips++;

    //query 2

    //se actualiza el oldest time y el oldest endId
    if(bike->station[startId-1].oldest.oldestDateTime == NULL){

        bike->station[startId-1].oldest.oldestDateTime = malloc(strlen(startDate)+1);
        bike->station[startId-1].oldest.oldestDateTime = strcpy(bike->station[startId-1].oldest.oldestDateTime, startDate);
        bike->station[startId-1].oldest.oldestStationId = endId;
    }
    else if (strcmp(startDate, bike->station[startId-1].oldest.oldestDateTime) < 0){
        bike->station[startId-1].oldest.oldestDateTime = strcpy(bike->station[startId-1].oldest.oldestDateTime, startDate);
        bike->station[startId-1].oldest.oldestStationId = endId;
    }

    //query 4
    addVec(bike, startId, endId);

    //query 5
    addVecq5(bike, endId, startId, startDate, endDate);

}


size_t getRealDim(bikeADT bike){
    return bike->dim_station;
}

size_t getResv(bikeADT bike){
    return bike->resv_station;
}

size_t getUsed(bikeADT bike, size_t idx){
    return bike->station[idx].used;
}

/*-----------------------------------query 1--------------------------------------*/

size_t getMemberTrips(bikeADT bike, size_t pos){
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }
    return bike->station[pos].memberTrips;
}

size_t getCausalTrips(bikeADT bike, size_t pos){
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }
    return bike->station[pos].casualTrips;
}

size_t getAllTrips(bikeADT bike, size_t pos){
    if(pos > bike->dim_station){
        bike->errorFlag = POS_ERR;
    }
    return bike->station[pos].allTrips;
}

char * getStationName(bikeADT bike, size_t pos){
    if(bike->station[pos].nameStation == NULL){
        return NULL;
    }
    
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }

    if(bike->station[pos].nameStation == NULL){
        bike->errorFlag = POS_ERR;
    }

    return copyStr(bike->station[pos].nameStation);
}

void tripSort(bikeADT bike){
    size_t k=0;
    for (size_t i=0; i < bike->resv_station ; i++){
        if (bike->station[i].used == 1){
            free(bike->station[k].nameStation);  
            if(bike->station[i].nameStation != NULL){
                bike->station[k].nameStation = copyStr(bike->station[i].nameStation);
                if(i > bike->dim_station){
                    free(bike->station[i].nameStation);
                }
            }
            bike->station[k].memberTrips = bike->station[i].memberTrips;
            bike->station[k].allTrips = bike->station[i].allTrips;
            bike->station[k].casualTrips = bike->station[i].casualTrips;
            
            //bike->station[k].oldest = bike->station[i].oldest;

            bike->station[k].oldest.oldestDateTime = copyStr(bike->station[i].oldest.oldestDateTime);
            free(bike->station[i].oldest.oldestDateTime);

            bike->station[k].oldest.oldestEndStation = copyStr(bike->station[i].oldest.oldestEndStation);
            free(bike->station[i].oldest.oldestEndStation);

            bike->station[k].oldest.oldestStationId = bike->station[i].oldest.oldestStationId;

            bike->station[k].dim_most = bike->station[i].dim_most;
            bike->station[k].most_vec = realloc(bike->station[k].most_vec, 1*sizeof(TVecPopular));
            bike->station[k].most_vec[0].endStation = NULL;
            bike->station[k].most_vec[0].endStation = copyStr(bike->station[i].most_vec[0].endStation);
            bike->station[k].most_vec[0].endStationId = bike->station[i].most_vec[0].endStationId;
            bike->station[k].most_vec[0].endStationTrips = bike->station[i].most_vec[0].endStationTrips;
           
            if(i > bike->dim_station){
                free(bike->station[i].most_vec[0].endStation);
                free(bike->station[i].most_vec);
            }    

            bike->station[k].idStation = i+1;
            bike->station[k++].used = 1;
        }
        bike->station[i].used = 0;
    }
    

    bike->station = realloc(bike->station, k*sizeof(TVecStation)); 
    bike->dim_station = bike->resv_station = k;
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare);
}


/*-----------------------------------query 2---------------------------------------------------*/

size_t getOldestRoute(bikeADT bike, size_t pos){
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }
    return bike->station[pos].oldest.oldestStationId;
}

char * getOldestName(bikeADT bike, size_t pos){
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }

    if(bike->station[pos].nameStation == NULL){
        bike->errorFlag = TOK_ERR;
    }
    return copyStr(bike->station[pos].oldest.oldestEndStation);
}

char * getOldestDateTime(bikeADT bike, size_t pos){
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }

    if(bike->station[pos].nameStation == NULL){
        bike->errorFlag = TOK_ERR;
    }
        
    return copyStr(bike->station[pos].oldest.oldestDateTime);
}

/*-----------------------------------query 3----------------------------------------------------*/

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

char * getMonthOfTheYear(size_t month){
    char * months[]={"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    return months[month];
}

/*---------------------------------------query 4------------------------------------------------------*/


//agrega al vector los endId
void addVec(bikeADT bike, size_t startId, size_t endId){ 
    if(startId != endId){
		if(bike->station[startId-1].dim_most == bike->station[startId-1].resv_most){
			bike->station[startId-1].most_vec = realloc(bike->station[startId-1].most_vec, (bike->station[startId-1].resv_most + BLOCK)*sizeof(TVecPopular));
			bike->station[startId-1].resv_most += BLOCK;
            
            for(int i = bike->station[startId-1].dim_most ; i < bike->station[startId-1].resv_most ; i++){
		        bike->station[startId-1].most_vec[i].endStation = NULL;
                bike->station[startId-1].most_vec[i].endStationId = 0;
		        bike->station[startId-1].most_vec[i].endStationTrips = 0;
		    }
        }

		int i = 0;
		for( ; i < bike->station[startId-1].dim_most ; i++){
			if(bike->station[startId-1].most_vec[i].endStationId == endId){ 
				bike->station[startId-1].most_vec[i].endStationTrips++;
				return;
			}
		}
        
        bike->station[startId-1].most_vec[i].endStationId = endId;
		bike->station[startId-1].most_vec[i].endStationTrips++;
		bike->station[startId-1].dim_most++;
		return;
    }
}

static int compare_most(const void *a, const void *b){
    TVecPopular *station1 = (TVecPopular *)a;
    TVecPopular *station2 = (TVecPopular *)b;
    int cmp = 0;
    if (station1->endStationTrips < station2->endStationTrips){
        cmp = 1;
    } else if (station1->endStationTrips > station2->endStationTrips){
        cmp = -1;
    }
    if (!cmp)
        cmp = 0;
    
    return cmp;
}

void sortMostPopularVec(bikeADT bike){
    for(int i = 0 ; i < bike->resv_station ; i++){
        if(bike->station[i].used){
            qsort(bike->station[i].most_vec, bike->station[i].dim_most ,sizeof(TVecPopular), compare_most); 
        }
    }
}

void addNameToVec(bikeADT bike, size_t pos){ 
    if(bike->resv_station < pos){
        bike->errorFlag = POS_ERR;
    }
    

    if(!bike->station[pos].dim_most){
        return;
    }else{
        if(bike->station[bike->station[pos].most_vec[0].endStationId-1].nameStation == NULL){
            bike->errorFlag = TOK_ERR;
        }

        bike->station[pos].most_vec = realloc(bike->station[pos].most_vec, sizeof(TVecPopular));
        bike->station[pos].most_vec[0].endStation = copyStr(bike->station[bike->station[pos].most_vec[0].endStationId-1].nameStation);
    }
}

void addNameToOldest(bikeADT bike, size_t pos){
    if(bike->resv_station < pos){
        bike->errorFlag = POS_ERR;
    }
    bike->station[pos].oldest.oldestEndStation = copyStr(bike->station[bike->station[pos].oldest.oldestStationId-1].nameStation);
}



size_t getMostPopRouteTrips(bikeADT bike, size_t pos){ 
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }
    return bike->station[pos].most_vec[0].endStationTrips;
}

char * getMostPopRouteEndStation(bikeADT bike, size_t pos){ 
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }

    if(bike->station[pos].most_vec[0].endStation == NULL){
        return NULL;
    }

    return copyStr(bike->station[pos].most_vec[0].endStation);
}


void sortAlpha(bikeADT bike){
    qsort(bike->station, bike->dim_station, sizeof(TVecStation), compare_stationData);
}

/*---------------------------------------query 5------------------------------------------------------*/


void addVecq5(bikeADT bike, size_t endId, size_t startId, char * startDate, char * endDate){
    int c = getMonth(startDate, endDate); 
    if(endId == startId && c != -1){
        if(bike->month[c].dimStat == bike->month[c].resStat){
            bike->month[c].circularStations = realloc(bike->month[c].circularStations, (bike->month[c].dimStat + BLOCK)* sizeof(stations)); 
            bike->month[c].resStat += BLOCK; 
            for(int i = bike->month[c].dimStat; i < bike->month[c].resStat; i++){
                bike->month[c].circularStations[i].nameStation = NULL;
                bike->month[c].circularStations[i].circularTrips = 0; 
                bike->month[c].circularStations[i].startId = 0; 
            }
            bike->month[c].dimStat++; 
        }
        int i; 
        for(i = 0; i < bike->month[c].dimStat; i++){
            if(bike->month[c].circularStations[i].startId == startId){
                bike->month[c].circularStations[i].circularTrips++; 
                return; 
            }
        }
        bike->month[c].circularStations[i].startId = startId; 
        bike->month[c].circularStations[i].circularTrips++; 
        bike->month[c].dimStat++; 
        return; 
    }
}

void sortCircularVec(bikeADT bike){
    for(int i = 0 ; i < MONTH ; i++){
        qsort(bike->month[i].circularStations, bike->month[i].dimStat , sizeof(stations), compare_circular); 
    }
}

void addNameToVecQ5(bikeADT bike, size_t pos){
    if(bike->dim_station < pos){
        bike->errorFlag = POS_ERR;
    }
    if(!bike->month[pos].dimStat){
        return;
    }else{
        int aux = MENOR(NAME_DISPLAY, bike->month[pos].dimStat);

        bike->month[pos].circularStations = realloc(bike->month[pos].circularStations, aux*sizeof(stations));

        for(int i = 0 ; i < aux ; i++){
            if(bike->station[bike->month[pos].circularStations[i].startId-1].nameStation == NULL){
                bike->errorFlag = TOK_ERR;
            }
            bike->month[pos].circularStations[i].nameStation = copyStr(bike->station[bike->month[pos].circularStations[i].startId-1].nameStation);
        }
    }
}

size_t getDimMonthStations(bikeADT bike, size_t pos){
    return MENOR(bike->month[pos].dimStat, NAME_DISPLAY);
}

char * getCircularName(bikeADT bike, size_t month, size_t pos){
    if(bike->month[month].circularStations[pos].nameStation == NULL){
        bike->errorFlag = POS_ERR;
    }
    return copyStr(bike->month[month].circularStations[pos].nameStation);
}


/*----------------------FREES-----------------------------*/

void freeADT(bikeADT bike){ 
    if(bike == NULL){
        bike->errorFlag = MEMO_ERR;
    }
    for (size_t i = 0; i < bike->dim_station ; i++){
        free(bike->station[i].nameStation);
        free(bike->station[i].oldest.oldestDateTime);
        free(bike->station[i].oldest.oldestEndStation);
        free(bike->station[i].most_vec[0].endStation);
        free(bike->station[i].most_vec);
    }

    
    for(int i = 0 ; i < MONTH ; i++){
        for(int j = 0 ; j < getDimMonthStations(bike, i)  ; j++){
            free(bike->month[i].circularStations[j].nameStation);
        }
        free(bike->month[i].circularStations);
    }
    
    free(bike->station);
}
