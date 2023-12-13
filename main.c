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
FILE *newFile(const char *inputFile);

int main(int argc, char *argv[])
{
    size_t formatDetect;
    size_t yearFrom = 0, yearTo = 0;

    bikeADT bike = new ();

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

    freeADT(bike);

    return 0;
}

void query1(bikeADT bike)
{
    tripSort(bike);

    char * station_name;

    FILE *file = newFile("query1.html");
    if (file == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 1</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Station Name</th>\n<th>Member Trips</th>\n<th>Casual Trips</th>\n<th>All Trips</th>\n</tr>\n");
    for (size_t i = 0; i < getRealDim(bike); i++)
    {
        station_name = getStationName(bike, i);
        fprintf(file, "<tr>\n<td>%s</td>\n<td>%ld</td>\n<td>%ld</td>\n<td>%ld</td>\n</tr>\n", station_name, getMemberTrips(bike, i), getCausalTrips(bike, i), getAllTrips(bike, i));
        free(station_name);
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
}

void query2(bikeADT bike)
{
    char * station_name;
    char * oldest_date;

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
        oldest_date = getOldestDateTime(bike, i);
        fprintf(file, "<tr>\n<td>%s</td>\n<td>%ld</td>\n<td>%s</td>\n</tr>\n", station_name, getOldestRoute(bike, i), oldest_date);
        free(station_name);
        free(oldest_date);
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
}

void query3(bikeADT bike)
{
    FILE *file = newFile("query3.html");
    if (file == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 3</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Day</th>\n<th>Started Trips</th>\n<th>Ended Trips</th>\n</tr>\n");
    for (size_t i = 0; i < 7; i++){
        fprintf(file, "<tr>\n<td>%s</td>\n<td>%ld</td>\n<td>%ld</td>\n</tr>\n", getDayOfTheWeek(i), getstartedTrips(bike, i, NULL), getEndedTrips(bike, i, NULL));
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
}
/*

bikeStation;mostPopRouteEndStation;mostPopRouteTrips
10e avenue / Holt;10e avenue / Masson;541
10e avenue / Masson;d'Orléans / Masson;1265
11e avenue / du Souvenir;Métro de la Concorde (Léo-Lacombe);29
12e avenue / St-Zotique;Métro de la Concorde (Léo-Lacombe);454
...


*/

void query4(bikeADT bike){
    FILE *file = newFile("query4.html");
    if (file == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }

    char * station_name;
    char * mostPopularEnd;

    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 4</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Station Name</th>\n<th>Most Popular Route</th>\n<th>Most Popular Route Trips</th>\n</tr>\n");
    for (size_t i = 0; i < getRealDim(bike); i++){
        if (getUsed(bike, i)){
            station_name = getStationName(bike, i);
            mostPopularEnd = getMostPopRouteEndStation(bike, i);
            fprintf(file, "<tr>\n<td>%s</td>\n<td>%s</td>\n<td>%ld</td>\n</tr>\n", station_name, mostPopularEnd, getMostPopRouteTrips(bike, i));
            free(station_name);
            free(mostPopularEnd);
        }
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
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
        }
    }
    
    fclose(file);
}


FILE *newFile(const char *inputFile){
    FILE *new = fopen(inputFile, "wt");
    return new;
}
