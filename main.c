#include <stdio.h>
#include "htmlTable.h"
#include "bikes.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define SEMICOLONS 4
#define DATE_SIZE 16

#ifdef NYC
    #define FORMAT 1
#else
    #define FORMAT 0
#endif


void nameReader(bikeADT bike, const char *inputFile, size_t *formatDetect);

bikeADT csvReader(const char *inputFile, size_t yearFrom, size_t yearTo, size_t *formatDetect);

void query1(bikeADT bike);
void query2(bikeADT bike);
void query3(bikeADT bike);
void query4(bikeADT bike);
void query5(bikeADT bike);

FILE *newFile(const char *inputFile);

int main(int argc, char *argv[]){
    size_t formatDetect;
    size_t yearFrom = 0, yearTo = 0;

    bikeADT bike = new();

    if (argc < 3){
        fprintf(stderr, "Invalid arguments\n");
        exit(ARG_ERR);
    }
    if (argc == 4){
        yearFrom = atoi(argv[3]);
        yearTo = atoi(argv[3]);
    }
    
    bike = csvReader(argv[1], yearFrom, yearTo, &formatDetect);
    nameReader(bike, argv[2], &formatDetect);
    
    if (bike == NULL){
        fprintf(stderr, "Memory error");
        exit(MEMO_ERR);
    }

    query1(bike);
    
    if(getErrorFlag(bike) != 0)
        errorReturn(bike);

    query2(bike);

    if(getErrorFlag(bike) != 0)
        errorReturn(bike);

    query3(bike);

    if(getErrorFlag(bike) != 0)
        errorReturn(bike);
    
    query4(bike);

    if(getErrorFlag(bike) != 0)
        errorReturn(bike);
    
    query5(bike);

    if(getErrorFlag(bike) != 0)
        errorReturn(bike);

    freeADT(bike);
    free(bike);

    return 0;
}

void query1(bikeADT bike)
{
    tripSort(bike);
    FILE * query1File = newFile("Query1.csv");
    
    if(query1File == NULL){
        fprintf(stderr,"Error al crear archivo out/Query1\n");
        exit(OPEN_ERR);
    }

    fputs("bikeStation;memberTrips;casualTrips;allTrips\n",query1File);

    htmlTable table = newTable("Query1.html", 4, "Station Name", "Member Trips", "Casual Trips", "All Trips");

    char * station_name;
    size_t MemberTrips;
    size_t CasualTrips;
    size_t AllTrips;

    char auxMember[10];
    char auxCasual[10];
    char auxAllTrips[10];

    for(size_t i = 0; i < getRealDim(bike); i++){
        station_name = getStationName(bike, i);
        MemberTrips = getMemberTrips(bike, i);
        CasualTrips = getCausalTrips(bike, i);
        AllTrips = getAllTrips(bike, i);

        sprintf(auxMember, "%zu", MemberTrips);
        sprintf(auxCasual, "%zu", CasualTrips);
        sprintf(auxAllTrips, "%zu", AllTrips);
        
        fprintf(query1File, "%s;%zu;%zu;%zu\n", station_name, MemberTrips, CasualTrips, AllTrips);

        addHTMLRow(table, station_name, auxMember, auxCasual, auxAllTrips);
        free(station_name);
    }
    fclose(query1File);
    closeHTMLTable(table); 
}

void query2(bikeADT bike){

    sortAlpha(bike);

    FILE * query2File = newFile("Query2.csv");
    if(query2File==NULL){
        fprintf(stderr,"Error al crear archivo Query2\n");
        exit(CREA_ERR);
    }

    fputs("stationName;oldestRoute;oldestDateTime\n", query2File);
    htmlTable table= newTable("Query2.html", 3, "Station Name", "Oldest Route", "Oldest Date Time");

    char * station_name;
    char * station_end;
    char * oldest_date;


   
   // DD/MM/YYYY HH:mm:ss

    for(int i = 0; i < getRealDim(bike); i++) {
        station_name = getStationName(bike, i);
        station_end = getOldestName(bike, i);
        oldest_date = getOldestDateTime(bike, i);

        char * aux = malloc(DATE_SIZE+1);
        aux = strncpy(aux, oldest_date, DATE_SIZE);
        aux[DATE_SIZE] = 0;

        fprintf(query2File, "%s;%s;%s\n", station_name, station_end, aux);

        addHTMLRow(table, station_name, station_end, aux);
        free(station_name);
        free(station_end);
        free(oldest_date);
        free(aux);
    }

    fclose(query2File);
    closeHTMLTable(table);
}


void query3(bikeADT bike){

    FILE * query3File = newFile("Query3.csv"); 
    if(query3File  == NULL){
        fprintf(stderr,"Error al crear archivo Query3\n");
        exit(CREA_ERR);
    }
    fputs("dayOfTheWeek; totalStartTrips; totalEndTrips\n", query3File);
    htmlTable table = newTable("Query3.html", 3, "day of the week" , "total Start Trips" , "total End Trips"); 
    
    size_t totalStart_trips; 
    size_t totalEnd_trips; 

    char auxTotalStart[10];
    char auxTotalEnd[10];

    char * days[] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
    

    for(size_t i = 1 ; i < WEEKS ; i++){
        int flag = 0;
        totalStart_trips = getstartedTrips(bike, i, &flag);
        totalEnd_trips = getEndedTrips(bike, i, &flag);
        if(flag){
            exit(1);
        }

        sprintf(auxTotalStart, "%zu", totalStart_trips);
        sprintf(auxTotalEnd, "%zu", totalEnd_trips);

        fprintf(query3File, "%s;%zu;%zu\n", days[i], totalStart_trips, totalEnd_trips);
        
        addHTMLRow(table, days[i], auxTotalStart, auxTotalEnd);
    }
    

    int flag = 0;
    totalStart_trips = getstartedTrips(bike, 0, &flag);
    totalEnd_trips = getEndedTrips(bike, 0, &flag);
    if(flag){
       exit(1);
    }

    sprintf(auxTotalStart, "%zu", totalStart_trips);
    sprintf(auxTotalEnd, "%zu", totalEnd_trips);

    fprintf(query3File, "%s;%zu;%zu\n", days[0], totalStart_trips, totalEnd_trips);
        
    addHTMLRow(table, days[0], auxTotalStart, auxTotalEnd);



    fclose(query3File);
    closeHTMLTable(table); 
}

void query4(bikeADT bike){
    FILE * query4File = newFile("Query4.csv");

    if(query4File == NULL){
        fprintf(stderr,"Error al crear archivo out/Query4\n");
        exit(OPEN_ERR);
    }

    fputs("bikeStation;mostPopRouteEndStation;mostPopRouteTrips\n",query4File);

    htmlTable table = newTable("Query4.html", 3, "Bike Station", "Most Pop. Route End Station", "Most Pop. Route Trips");

    char * station_name;
    char * mostPopularEnd;
    size_t mostPopularTrips;

    char auxMostPopularTrips[10];

    for(size_t i = 0; i < getRealDim(bike); i++){
        station_name = getStationName(bike, i);
        mostPopularEnd = getMostPopRouteEndStation(bike, i);
        if(mostPopularEnd == NULL){
            exit(TOK_ERR);
        }
        mostPopularTrips = getMostPopRouteTrips(bike, i);

        sprintf(auxMostPopularTrips, "%zu", mostPopularTrips);


        fprintf(query4File, "%s;%s;%zu;\n", station_name, mostPopularEnd, mostPopularTrips);

        addHTMLRow(table, station_name, mostPopularEnd, auxMostPopularTrips);
        free(station_name);
        free(mostPopularEnd);
    }
    fclose(query4File);
    closeHTMLTable(table);
}


void query5(bikeADT bike){
    
    FILE *query5File = newFile("Query5.csv");
    if (query5File == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fputs("month;loopsTop1St;loopsTop2St;loopsTop3St\n",query5File);
    htmlTable table = newTable("Query5.html", 4, "Month", "LoopsTop1St", "LoopsTop2St", "LoopsTop3St");

    char * station_name1;
    char * month = NULL;

    for (size_t i = 0; i < MONTH ; i++){
        month = getMonthOfTheYear(i);

        char * vec[NAME_DISPLAY] = {NULL};
        int flag[NAME_DISPLAY] = {0};

        fprintf(query5File, "%s;", month);
        for(size_t j = 0 ; j < getDimMonthStations(bike, i) ; j++){
                station_name1 = getCircularName(bike, i, j);
                if(station_name1 != NULL){
                    vec[j] = copyStr(station_name1);
                    flag[j]++;
                    fprintf(query5File, "%s;",station_name1);
                }else{
                    vec[j] = "";
                    fprintf(query5File, "%s;", "");
                }
                free(station_name1);
        }
        if(!getDimMonthStations(bike, i)){
            for(int i = 0 ; i < NAME_DISPLAY ; i++){
                vec[i] = "";
            }
        }

        addHTMLRow(table, month, vec[0], vec[1], vec[2]);
        fprintf(query5File, "\n");

        for(int i = 0 ; i < NAME_DISPLAY ; i++){
            if(flag[i]){
                free(vec[i]);
            }
        }
    }
    fclose(query5File);
    closeHTMLTable(table); 
}


bikeADT csvReader(const char *inputFile, size_t yearFrom, size_t yearTo, size_t *formatDetect)
{
    FILE *file = fopen(inputFile, "r");
    if (file == NULL){
        fprintf(stderr, "Error opening file %s\n", inputFile);
        exit(OPEN_ERR);
    }

    bikeADT bike = new();
    if (bike == NULL){
        fprintf(stderr, "Memory error\n");
        exit(MEMO_ERR);
    }

    char actualRead[MAXCHAR];
    if (fgets(actualRead, sizeof(actualRead), file) == NULL){
        perror("Error reading file");
        fclose(file);
        return NULL;
    }

    //*formatDetect = (strstr(actualRead, "started_at") != NULL) ? 1 : 0; //NYC 1  - MON 0
    //size_t flagError = 0;

    *formatDetect = FORMAT;

    char *startDate;
    size_t startId;
    char *endDate;
    size_t endId;
    int isMember;

    while (fgets(actualRead, MAXCHAR, file) != NULL){
        if (*formatDetect){
            char *token = strtok(actualRead, ".");
            startDate = token;
            token = strtok(NULL, ";");

            token = strtok(NULL, ";");
            startId = strtoul(token, NULL, 10);

            token = strtok(NULL, ".");
            endDate = token;
            token = strtok(NULL, ";");

            token = strtok(NULL, ";");
            endId = strtoul(token, NULL, 10);

               
            token = strtok(NULL, ";");

            token = strtok(NULL, ";");
            isMember = (token[0] == 'm'); 

                
            }else{ 

                char *token = strtok(actualRead, ";");
                startDate = token;

                token=strtok(NULL, ";");
                startId = strtoul(token, NULL, 10);

                token = strtok(NULL, ";");
                endDate = token;

                token = strtok(NULL, ";");
                endId = strtoul(token, NULL, 10);

                token = strtok(NULL, ";");
                isMember = strtoul(token, NULL, 10);

                
            }
            putStation(bike, startDate, startId, endDate, endId, isMember, yearFrom, yearTo);
            

    }

    /*
    for(int i = 0 ; i < getResv(bike) ; i++){
        if(getUsed(bike, i)){
            reallocEachVec(bike, i);
        }
    }
    */

    sortMostPopularVec(bike);
    sortCircularVec(bike);

    fclose(file);
    return bike;
}

void nameReader(bikeADT bike, const char *inputFile, size_t *formatDetect){
    FILE *file = fopen(inputFile, "r");
    
    if (file == NULL){
        fprintf(stderr, "Error opeing file %s\n", inputFile);
        exit(OPEN_ERR);
    }
    
    char actualRead[MAXCHAR];
    fscanf(file, "%s\n", actualRead); 

    size_t stationId;
    char * stationName;

    while (fgets(actualRead, MAXCHAR, file) != NULL){
        
        //caso NYC
        if (*formatDetect){
            char *token = strtok(actualRead, ";");
            if(token != NULL){
                stationName = malloc((strlen(token)+1));
                strcpy(stationName, token);
            }else{
                fprintf(stderr, "token name NULL");
                exit(1);
            }
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");

            token = strtok(NULL, "\n");
            stationId = strtoul(token, NULL, 10);
        }
        //caso MON
        else{
            char *token = strtok(actualRead, ";");
            //stationId = atoi(token);
            stationId = strtoul(token, NULL, 10);

            token = strtok(NULL, ";");

            if(token != NULL){
                stationName = malloc((strlen(token)+1));
                strcpy(stationName, token);
            }else{
                fprintf(stderr, "token name NULL");
                exit(1);
            }
            token = strtok(NULL, "\n");
        }

        string_cpy(bike, stationName, stationId);
        free(stationName);
    }    

    for(int i = 0 ; i < getResv(bike) ; i++){
        if(getUsed(bike, i)){
            addNameToVec(bike, i);
            addNameToOldest(bike, i);
            //se llama para cada posicion en el vector station donde el used es 1
        }
    }
    for(int i = 0 ; i < MONTH ; i++){
        addNameToVecQ5(bike, i);
    }

    fclose(file);
}


FILE *newFile(const char *inputFile){
    FILE *new = fopen(inputFile, "wt");
    return new;
}

void errorReturn(bikeADT bike){
    switch(getErrorFlag(bike)){
        case ARG_ERR:
            printf("Error en la cantidad de argumentos.\n");
            break;
        case OPEN_ERR:
            printf("Error al abrir archivo.\n");
            break;
        case CREA_ERR:
            printf("Error al crear archivo");
            break;
        case MEMO_ERR:
            printf("Error al crear memoria.\n");
            break;
        case TOK_ERR:
            printf("Error de lectura.\n");
            break;
        case POS_ERR:
            printf("Error de acceso: Posicion > Dimension.\n");
            break;
        case PAR_ERR:
            printf("Error al parsear Linea.\n");
            break;
        case ENT_ERR:
            printf("Error de entrada.\n");
            break;
        case CPY_ERR:
            printf("Error en la funcion strcpy.\n");
            break;
    }
    exit(getErrorFlag(bike));
}
