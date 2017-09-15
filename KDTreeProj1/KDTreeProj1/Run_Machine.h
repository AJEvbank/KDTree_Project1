#ifndef Run_Machine

#define Run_Machine

#include "myHeader.h"

/* Data Structures */

enum isNumStates {
	INITIAL,
	PLUSORMINUS,
	ZERO,
	NUMBER,
	DECIMAL,
	ERROR
};


/* Run State Machine Functions */

void runMachine(int, const char *[]);

int isNumber(const char *);

void writeResults(int dim, int ndata, double* data, int* cluster_assign);

#endif
