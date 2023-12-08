
#ifndef __bikes_h_
#define __bikes_h_
#include <stdio.h>
#include <errno.h>

typedef struct bikeCDT * bikeADT;

bikeADT new(void);


/*query 1*/

/*query 2*/ 

/*query 3*/

size_t startedTrips(bikeADT bike, int day);

size_t endedTrips(bikeADT bike, int day);

/*query 4*/


#endif
