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

