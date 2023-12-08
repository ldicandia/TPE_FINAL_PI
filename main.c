#include <stdio.h>
#include "htmlTable.h"
#include "bikes.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define SEMICOLONS 4

void putName(bikeADT bike, const char * inputFile);

bikeADT putCsv(const char * inputFile, size_t yearFrom, size_t yearTo);

//void query1(bikeADT bike);
//void query2(bikeADT bike);
//void query3(bikeADT bike);
//void query4(bikeADT bike);

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

    //query1(bike);
    //query2(bike);
    //query3(bike);
    //query4(bike);

    freeADT(bike);

    return 0;
}

// ejemplo csv nyc
// fecha alquiler          ; start id; fecha devolucion;       endId;  biketype;  m o c;
// 2022-11-17 19:05:10.000000;489509;2022-11-17 19:07:30.000000;490309;classic_bike;member

// ejemplo csv mon
// fecha alquiler;   start id; fecha devolucion; endId ; m o c;
// 2021-09-20 06:31:28;348;2021-09-20 07:02:22;332;1

//funcion que lee los archivos csv en cualquiera de los dos formatos y guarda los datos en variables
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
    size_t semiColons=0;
    
     if (fgets(actualRead, sizeof(actualRead), file) == NULL) {
         perror("Error reading file");
         fclose(file);
         return 0;
     }

     fclose(file);

     // Contar los ";" en la primera línea
     size_t count = SEMICOLONS;
     for (size_t i = 0; actualRead[i] != '\0'; i++) {
         if (actualRead[i] == ';') {
             count--;
         }
     }

     if(count==0){
        semiColons=1;
     }
 

    fscanf(file, "%s\n", actualRead); /*salta la primer linea*/

    char * startDate;
    size_t startId;
    char * endDate;
    size_t endId;   
    size_t isMember;
    
    size_t flagError = 0;

    while(fgets(actualRead, MAXCHAR, file) != NULL){
        //copia startDate
        char * token = strtok(actualRead, ";");
        if (token != NULL) {
            startDate = malloc(strlen(token)+1);
            if (startDate != NULL) {
                strcpy(startDate, token);
            }
         else {
            fprintf(stderr, "Memory error");
            exit(MEMO_ERR);
        }
        //copia startId
        startId = atoi(strtok(NULL, ";"));
        //copia endDate
        token = strtok(NULL, ";");
        if (token != NULL){
            endDate = malloc(strlen(token) + 1);
            if (endDate != NULL){
                strcpy(endDate, token);
            }
            else{
                fprintf(stderr, "Memory error");
                exit(MEMO_ERR);
            }
        }
        //copia endId
        endId = atoi(strtok(NULL, ";"));
//saltea el biketype si es q esta
        if(semiColons==1){
            strtok(NULL, ";");
        }
        isMember = atoi(strtok(NULL, "\n"));
        
        putStation(bike, startDate,startId, endDate, endId, isMember, yearFrom, yearTo);
        
        free(startDate);
        free(endDate);
    }

        putMatrix(bike, startId, endId, &flagError);

        if (flagError == MEMO_ERR){
            fprintf(stderr, "NULL token error");
            exit(TOK_ERR);
        }

            free(startDate);
        }
    fclose(file);
   
    return bike;
    
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
        
        bike = string_cpy(bike, stationName, stationId); //copia al adt el nombre de la estacion

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