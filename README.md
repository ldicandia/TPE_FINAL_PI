# 2do CUATRIMESTRE 2023
## _Segunda Fecha 19/12_

#### AUTORES:
- _Franco Ferrari 63094_
- _Keoni Dubovitsky 62815_
- _Mateo Pirola 62810_
- _Lucas Di Candia 63212_


## Compilacion y ejecucion del programa.
### Compilacion
Para compilar el proyecto ingrese a su directorio con los archivos fuente, correr:
```
make all
```

Si desea unicamente crear el proyecto para la informacion de tipo "Montreal", entonces correr:
```
make mon
```
Caso contrario, para la informacion de tipo "New York City", correr:
```
make nyc
```

### Ejecucion
Una vez compilado el programa, para ejecutarlo se debe correr de la siguiente manera.
```
./bikeSharingMON bikesMON.csv stationsMON.csv yearFrom yearTo
```
_Siendo, bikeSharingMON el nombre del ejecutable, bikesMON el .csv que contiene la informacion de rentas para Montreal, stationsMON la informacion de estaciones que contiene la misma ciudad y yearFrom - yearTo los anios que se desea buscar_
```
./bikeSharingNYC bikesNYC.csv stationsNYC.csv yearFrom yearTo
```
_Siendo, bikeSharingNYC el nombre del ejecutable, bikesNYC el .csv que contiene la informacion de rentas para New York City, stationsNYC la informacion de estaciones que contiene la misma ciudad y yearFrom - yearTo los anios que se desea buscar_

## Queries

### Query1
Guarda en un archivo .csv y .html la informacion de la consulta dada por la cantidad de viajes totales por cada estacion, indicando tambien cuantos de ellos fueron hechos por 
personas miembro, y cuantos por personas casuales.
### Query2
Guarda en un archivo .csv y .html la informacion de la consulta dada por la fecha del viaje mas antiguo registrado y la estacion donde se concreto, para cada estacion
de inicio.
### Query3 
Guarda en unarchivo .csv y .html la informacion de la consulta dada por la cantidad de viajes iniciados y la cantidad de viajes terminados por cada dia de la semana.
### Query4
Guarda en un archivo .csv y .html la informacion de la consulta dada por la estacion donde mas viajes se terminaron desde cada estacion, y cuantos viajes terminaron en ella.
### Query5 
Guarda en un archivo .csv y .html la informacion de la consulta dada por las tres estaciones donde mas viajes circulares ocurrieron por cada mes del anio.

## ACLARACION

al ejecutar da error de leak de memoria el LeakSanitizer pero genera los archivos, los leaks no los pudimos encontrar.
```
./bikeSharingNYC bikesNYC.csv stationsNYC.csv 

==1522827==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 432 byte(s) in 1 object(s) allocated from:
    #0 0x7fe3415fd411 in __interceptor_calloc /usr/src/debug/gcc/libsanitizer/asan/asan_malloc_linux.cpp:77
    #1 0x561180b3f674 in new (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingNYC+0x7674)
    #2 0x561180b3c4c6 in main (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingNYC+0x44c6)
    #3 0x7fe34137a2cf  (/usr/lib/libc.so.6+0x232cf)

SUMMARY: AddressSanitizer: 432 byte(s) leaked in 1 allocation(s).
```
en este dice que no liberamos el bikeADT bike pero lo liberamos al final del main.
```
./bikeSharingMON bikesMON.csv stationsMON.csv
==1524035==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 22467 byte(s) in 692 object(s) allocated from:
    #0 0x7f9385c72a89 in __interceptor_malloc /usr/src/debug/gcc/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x55752ddb6538 in copyStr (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x7538)
    #2 0x55752ddba609 in addNameToVec (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0xb609)
    #3 0x55752ddb5aa3 in nameReader (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x6aa3)
    #4 0x55752ddb3646 in main (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x4646)
    #5 0x7f93859ef2cf  (/usr/lib/libc.so.6+0x232cf)

Direct leak of 432 byte(s) in 1 object(s) allocated from:
    #0 0x7f9385c72411 in __interceptor_calloc /usr/src/debug/gcc/libsanitizer/asan/asan_malloc_linux.cpp:77
    #1 0x55752ddb6674 in new (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x7674)
    #2 0x55752ddb34c6 in main (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x44c6)
    #3 0x7f93859ef2cf  (/usr/lib/libc.so.6+0x232cf)

Direct leak of 46 byte(s) in 2 object(s) allocated from:
    #0 0x7f9385c72a89 in __interceptor_malloc /usr/src/debug/gcc/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x55752ddb5dfb in string_cpy (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x6dfb)
    #2 0x55752ddb5a29 in nameReader (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x6a29)
    #3 0x55752ddb3646 in main (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x4646)
    #4 0x7f93859ef2cf  (/usr/lib/libc.so.6+0x232cf)

Direct leak of 24 byte(s) in 1 object(s) allocated from:
    #0 0x7f9385c717ea in __interceptor_realloc /usr/src/debug/gcc/libsanitizer/asan/asan_malloc_linux.cpp:85
    #1 0x55752ddba4c0 in addNameToVec (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0xb4c0)
    #2 0x55752ddb5aa3 in nameReader (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x6aa3)
    #3 0x55752ddb3646 in main (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x4646)
    #4 0x7f93859ef2cf  (/usr/lib/libc.so.6+0x232cf)

Indirect leak of 58 byte(s) in 1 object(s) allocated from:
    #0 0x7f9385c72a89 in __interceptor_malloc /usr/src/debug/gcc/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x55752ddb6538 in copyStr (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x7538)
    #2 0x55752ddba609 in addNameToVec (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0xb609)
    #3 0x55752ddb5aa3 in nameReader (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x6aa3)
    #4 0x55752ddb3646 in main (/afs/it.itba.edu.ar/home/ldicandia/Repos/TPE_FINAL_PI/bikeSharingMON+0x4646)
    #5 0x7f93859ef2cf  (/usr/lib/libc.so.6+0x232cf)

SUMMARY: AddressSanitizer: 23027 byte(s) leaked in 697 allocation(s).

```
este tiene el mismo "error" del bikeADT pero tiene mas por copyStr que no pudimos encontrar en ningun lado ya que cada vez que se llamo a copyStr se realizo 
el debido free despues.

