#include <stdio.h>
#include "htmlTable.h"
#include "bikes.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define SEMICOLONS 4
#define LEN_HOURS 20
#define MAX_NAME 50

void nameReader(bikeADT bike, const char * inputFile, size_t * semiColons);

bikeADT csvReader(const char * inputFile, size_t yearFrom, size_t yearTo, size_t * semiColons);

void query1(bikeADT bike);
void query2(bikeADT bike);
void query3(bikeADT bike);
void query4(bikeADT bike);
FILE * newFile(const char * inputFile);

int main(int argc, char * argv[]){
    size_t semiColons;
    size_t yearFrom=0, yearTo=0;
    
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
        bike = csvReader(argv[2], yearFrom, yearTo, &semiColons);
        nameReader(bike, argv[1], &semiColons);
    }else if (strcmp(argv[1], argv[2]) < 0){
        bike = csvReader(argv[1], yearFrom, yearTo, &semiColons);
        nameReader(bike, argv[2], &semiColons);
    }else{
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

void query1(bikeADT bike){
    FILE * file = newFile("query1.html");
    if (file == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 1</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Station Name</th>\n<th>Member Trips</th>\n<th>Casual Trips</th>\n<th>All Trips</th>\n</tr>\n");
    for (size_t i = 0; i < getRealDim(bike); i++){
        fprintf(file, "<tr>\n<td>%s</td>\n<td>%ld</td>\n<td>%ld</td>\n<td>%ld</td>\n</tr>\n", getStationName(bike, i), getMemberTrips(bike, i), getCausalTrips(bike, i), getAllTrips(bike, i));
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
}

void query2(bikeADT bike){
    FILE * file = newFile("query2.html");
    if (file == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 2</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Station Name</th>\n<th>Oldest Route</th>\n<th>Oldest Date Time</th>\n</tr>\n");
    for (size_t i = 0; i < getRealDim(bike); i++){
        fprintf(file, "<tr>\n<td>%s</td>\n<td>%ld</td>\n<td>%s</td>\n</tr>\n", getStationName(bike, i), getOldestRoute(bike, i), getOldestDateTime(bike, i));
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
}

void query3(bikeADT bike){
    FILE * file = newFile("query3.html");
    if (file == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 3</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Day</th>\n<th>Started Trips</th>\n<th>Ended Trips</th>\n</tr>\n");
    for (size_t i = 0; i < 7; i++){
        fprintf(file, "<tr>\n<td>%ld</td>\n<td>%ld</td>\n<td>%ld</td>\n</tr>\n", i, getstartedTrips(bike, i, NULL), getEndedTrips(bike, i, NULL));
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
}   

void query4(bikeADT bike){
    FILE * file = newFile("query4.html");
    if (file == NULL){
        fprintf(stderr, "Error creating file");
        exit(CREA_ERR);
    }
    fprintf(file, "<!DOCTYPE html>\n<html>\n<head>\n<title>Query 4</title>\n</head>\n<body>\n");
    fprintf(file, "<table border=\"1\">\n");
    fprintf(file, "<tr>\n<th>Station Name</th>\n<th>Most Popular Route End Station</th>\n<th>Most Popular Route Trips</th>\n</tr>\n");
    for (size_t i = 0; i < getRealDim(bike); i++){
        fprintf(file, "<tr>\n<td>%s</td>\n<td>%s</td>\n<td>%ld</td>\n</tr>\n", getStationName(bike, i), getMostPopRouteEndStation(bike, i), getMostPopRouteTrips(bike, i));
    }
    fprintf(file, "</table>\n</body>\n</html>");
    fclose(file);
}
// ejemplo csv nyc
// fecha alquiler          ; start id; fecha devolucion;       endId;  biketype;  m o c;
// 2022-11-17 19:05:10.000000;489509;2022-11-17 19:07:30.000000;490309;classic_bike;member

// ejemplo csv mon
// fecha alquiler;   start id; fecha devolucion; endId ; m o c;
// 2021-09-20 06:31:28;348;2021-09-20 07:02:22;332;1

//funcion que lee los archivos csv en cualquiera de los dos formatos y guarda los datos en variables


bikeADT csvReader(const char *inputFile, size_t yearFrom, size_t yearTo, size_t *semiColons) {
    FILE *file = fopen(inputFile, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", inputFile);
        exit(OPEN_ERR);
    }

    bikeADT bike = new();
    if (bike == NULL) {
        fprintf(stderr, "Memory error\n");
        exit(MEMO_ERR);
    }

    char actualRead[MAXCHAR];
    (*semiColons) = 0;

    // Omite la primera línear
    if (fgets(actualRead, sizeof(actualRead), file) == NULL) {
        perror("Error reading file");
        fclose(file);
        return 0;
    }

    // Código para extraer solo la fecha
    char startDate[20], endDate[20];
    size_t startId, endId, isMember;
    char datetime[LEN_HOURS];  // Para almacenar la fecha y hora temporalmente

    while (fgets(actualRead, MAXCHAR, file) != NULL) {
        int result = sscanf(actualRead, "%19[^;];%zu;%19[^;];%zu;%zu", datetime, &startId, datetime, &endId, &isMember);
        if (result != 5) {
            fprintf(stderr, "Error parsing line: %s\n", actualRead);
            exit(1);
        }

        // Extraer solo la fecha de las cadenas de fecha y hora
        sscanf(datetime, "%10s", startDate);
        sscanf(datetime, "%10s", endDate);

        putStation(bike, startDate, startId, endDate, endId, isMember, yearFrom, yearTo);

        if (bike == NULL) {
            fprintf(stderr, "Memory error\n");
            exit(MEMO_ERR);
        }

        size_t flagError = 0;
        addMatrix(bike, startId, endId, &flagError);

        if (flagError == MEMO_ERR) {
            fprintf(stderr, "NULL token error\n");
            exit(TOK_ERR);
        }
    }

    fclose(file);

    return bike;
}


//ejemplo csv nyc
//nombre               ; latitud ; longitud ; id
//River Ave & E 151 St;40.822217;-73.928939;796704

//ejemplo csv mon
//id  ; nombre ; latitud ;longitud
//327;Sanguinet / de Maisonneuve;45.513405;-73.562594


void nameReader(bikeADT bike, const char * inputFile, size_t * semiColons){
    FILE * file = fopen(inputFile, "r");

        if(file == NULL){
            fprintf(stderr, "Error opeing file %s\n", inputFile);
            exit(OPEN_ERR);
        }
    
       
    char actualRead[MAXCHAR];

    fscanf(file, "%s\n", actualRead); /* salta la primer linea */

    size_t stationId;
    char * token;
    char stationName[MAX_NAME];
    

    while(fgets(actualRead, MAXCHAR, file) != NULL){
       int result = sscanf(actualRead, "%d;%49[^;]", &stationId, stationName);
       if (result != 2) {
            fprintf(stderr, "Error parsing line: %s\n", actualRead);
            exit(1);
        }
        string_cpy(bike, stationName, stationId);
    }

    fclose(file);

}

FILE * newFile(const char * inputFile){
    FILE * new = fopen(inputFile, "wt");
    return new;
}

