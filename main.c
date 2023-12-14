#include <stdio.h>
#include "htmlTable.h"
#include "bikes.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define SEMICOLONS 4


void nameReader(bikeADT bike, const char *inputFile, size_t *formatDetect);

bikeADT csvReader(const char *inputFile, size_t yearFrom, size_t yearTo, size_t *formatDetect);

void query1(bikeADT bike);
void query2(bikeADT bike);
void query3(bikeADT bike);
void query4(bikeADT bike);
void query5(bikeADT bike);

FILE *newFile(const char *inputFile);

int main(int argc, char *argv[])
{
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
    if (strcmp(argv[1], argv[2]) > 0){
        bike = csvReader(argv[2], yearFrom, yearTo, &formatDetect);
        nameReader(bike, argv[1], &formatDetect);
    }
    else if (strcmp(argv[1], argv[2]) < 0){
        bike = csvReader(argv[1], yearFrom, yearTo, &formatDetect);
        nameReader(bike, argv[2], &formatDetect);
    }
    else{
        fprintf(stderr, "Invalid arguments order\n");
        exit(ARG_ERR);
    }
    if (bike == NULL){
        fprintf(stderr, "Memory error");
        exit(MEMO_ERR);
    }
    query1(bike);
    query2(bike);
    query3(bike);
    query4(bike);
    query5(bike);

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

    /* sortAlpha(bike);

        FILE * query2File = newFile("Query2.csv");
        if(query2File==NULL){
            fprintf(stderr,"Error al crear archivo Query2\n");
            exit(CRERR);
        }

        fputs("stationName;oldestRoute;oldestDateTime\n", query2File);
        htmlTable table= newTable("Query2.html", 3, "Station Name", "Oldest Route", "Oldest Date Time");

        char * station_name;
        char * station_end;
        char * oldest_date;

        for(int i = 0; i < getRealDim(bike); i++) {
            char * station_name = getStationName(bike, i);
            char * station_end = getOldestName(bike, i);
            char * oldest_date = getOldestDateTime(bike, i);

            fprintf(query2File, "%s;%s;%s", station_name, station_end, oldest_date);

            addHTMLRow(table, station_name, station_end, oldest_date);
            free(station_name);
            free(station_end);
            free(oldest_date);
            }

        fclose(query2File);
        closeHTMLTable(table);
        }

*/

    char *station_name;
    char *station_end;
    char *oldest_date;
    sortAlpha(bike);
    FILE *file = newFile("query2.html");
    if (file == NULL)
    {
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    
    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 2</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Station Name</th>\n<th>Oldest Route</th>\n<th>Oldest Date Time</th>\n</tr>\n");
    for (size_t i = 0; i < getRealDim(bike); i++)
    {
        station_name = getStationName(bike, i);
        station_end = getOldestName(bike, i);
        oldest_date = getOldestDateTime(bike, i);
        fprintf(file, "<tr>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n</tr>\n", station_name, station_end, oldest_date);
        free(station_name);
        free(station_end);
        free(oldest_date);
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
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
        mostPopularEnd= getMostPopRouteEndStation(bike, i);
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


/* query 5 debe imprimir un html de la siguiente forma llamando a la funcion getCircularName
Salida de ejemplo para montreal: 

month;loopsTop1St;loopsTop2St;loopsTop3St
January;Empty;Empty;Empty
February;Empty;Empty;Empty

November;Crescent / de Maisonneuve;de la Commune / St-Sulpice;Parc Jean-Drapeau (Chemin Macdonald)
December;Empty;Empty;Empty
*/

void query5(bikeADT bike){
    
    FILE *query5File = newFile("Query5.csv");
    if (query5File == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fputs("month;loopsTop1St;loopsTop2St;loopsTop3St\n",query5File);
    htmlTable table = newTable("Query5.html", 4, "Month", "LoopsTop1St", "LoopsTop2St", "LoopsTop3St");

    char * station_name1;
    char * month;

    for (size_t i = 0; i < MONTH ; i++){
        month = getMonthOfTheYear(i);

        /*
        station_name1 = getCircularName(bike, i, 0);
        if(station_name1 == NULL){
            station_name1 = "";
        }
        station_name2 = getCircularName(bike, i, 1);
        if(station_name2 == NULL){
            station_name2 = "";
        }
        station_name3 = getCircularName(bike, i, 2);
        if(station_name3 == NULL){
            station_name3 = "";
        }

        fprintf(query5File, "%s;%s;%s;%s\n", month, station_name1, station_name2, station_name3);
        addHTMLRow(table, month, station_name1, station_name2, station_name3);
        */

        char * vec[3];

        fprintf(query5File, "%s;", month);
        for(size_t j = 0 ; j < getDimMonthStations(bike, i) ; j++){
                station_name1 = getCircularName(bike, i, j);
                if(station_name1 != NULL){
                    vec[j] = copyStr(station_name1);
                    fprintf(query5File, "%s;",station_name1);
                }else{
                    vec[j] = "";
                    fprintf(query5File, "%s;", "");
                }
                free(station_name1);
        }

        addHTMLRow(table, month, vec[0], vec[1], vec[2]);
        fprintf(query5File, "\n");

        //free(station_name1);
        //free(month);
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

    *formatDetect = (strstr(actualRead, "started_at") != NULL) ? 1 : 0; //NYC 1  - MON 0
    //size_t flagError = 0;

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
