#include <stdio.h>
#include "htmlTable.h"
#include "bikes.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void query1(bikeADT bike);
void query2(bikeADT bike);
void query3(bikeADT bike);
void query4(bikeADT bike);

int main(int argc, char * argv[]){
    size_t yearFrom=0, yearTo=0;
    
    bikeADT bike;

    if (strcmp(argv[1], argv[2]) > 0){
        bike = putCsv(argv[2], yearFrom, yearTo);
        putName(bike, argv[1]);
    }else if (strcmp(argv[1], argv[2]) < 0){
        bike = putCsv(argv[1], yearFrom, yearTo);
        putName(bike, argv[2]);
    }else{
        fprintf(stderr, "Invalid arguments order\n");
        exit(ARG_ERR);
    }

    query1(bike);
    query2(bike);
    query3(bike);
    query4(bike);

    freeADT(bike);

    return 0;
}

bikeADT putCsv(const char * inputFile, size_t yearFrom, size_t yearTo){
    FILE * file = fopen(inputFile, "rt");
        if(file == NULL){
            fprintf(stderr, "Error opeing file%s\n", inputFile);
            exit(OPEN_ERR);
        }

    bikeADT bike = new();

    if( bike == NULL ){
        fprintf(stderr, "Memory error");
        exit(MEMO_ERR);
    }

    char actualRead[MAXCHAR];

    fscanf(file, "%s\n", actualRead); /* salta la primer linea */

   

    char * bikeStart;
    size_t startId;
    size_t endId;    
    size_t flagError = 0;
    size_t isMember;

    while(fgets(actualRead, MAXCHAR, file) != NULL){
        char * token = strtok(actualRead, ";");
        if (token != NULL) {
            bikeStart = malloc(strlen(token)+1);
            if (bikeStart != NULL) {
                strcpy(bikeStart, token);
            }
         else {
            fprintf(stderr, "Memory error");
            exit(MEMO_ERR);
        }
        startId = atoi(strtok(NULL, ";"));
        strtok(NULL, ";");
        endId = atoi(strtok(NULL, ";")); 
        isMember = atoi(strtok(NULL, "\n"));

        putStation(bike, startId, endId, yearFrom, yearTo);
        putBikes(bike, startId, endId, &flagError);

        if (flagError == MEMO_ERR){   
            fprintf(stderr, "NULL token error");
            exit(TOK_ERR);
        }

        free(bikeStart);
    }
    fclose(file);
   
    return bike;
    
    }
}

void putName(bikeADT bike, const char * inputFile){
    FILE * file = fopen(inputFile, "rt");
        if(file == NULL){
            fprintf(stderr, "Error opeing file%s\n", inputFile);
            exit(OPEN_ERR);
        }
    char actualRead[MAXCHAR];

    fscanf(file, "%s\n", actualRead); /* salta la primer linea */

    size_t stationId;
    char * token;
    char * stationName;


    while(fgets(actualRead, MAXCHAR, file) != NULL){
        stationId = atoi(strtok(actualRead,";"));
        token = strtok(NULL,";");
        if(token != NULL){
            stationName = malloc((strlen(token)+1)); // * sizeof(char)
            if(stationName != NULL){
                strcpy(stationName, token);
            }else{
                fprintf(stderr, "Memory error");
                exit(MEMO_ERR);
            }
        }else{
            fprintf(stderr, "NULL token error");
            exit(TOK_ERR);
        }
        strtok(NULL, ";");//Salteo Latitud 
        strtok(NULL, "\n"); //y longitud.
        
        bike = strcpy(bike, stationName); 

        if(bike == NULL){ 
            fprintf(stderr, "Memory error");
            exit(MEMO_ERR);
        } 

        free(stationName);

    }

    fclose(file);

}


FILE * newFile(const char * inputFile){
    FILE * new = fopen(inputFile, "wt");
    return new;
}