#ifndef myHeader

#define myHeader


#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "ctype.h"
#include "string.h"
#include "DP_Array_Functions.h"
#include "Run_Machine.h"
#include "DEBUG.h"
#include "stack.h"

#define DIM 3
#define NDATA 45
#define MAX_DOUBLE 60.00
/* debugging macros */
#define DEBUG 0
#define PARTITION 0
#define PB_ANALYSIS 0
#define DATA_ANALYSIS 0
#define DEBUG_INITIALIZATION 0
#define KDTREE_INIT 0
#define WORKING_DATA_ARRAY 0
#define KDTREE_FINAL 0
#define QUERY_ANALYSIS 0
#define QUERY_ANALYSIS2 0
#define QUERY_ANALYSIS3 0
#define QUERY_ANALYSIS4 0
#define FINAL_RESULT 0

/*Data Structures*/

struct kdtree {

	int dim; /* the number of dimensions of each data point*/
	int ndata; /* the number of data points */
	unsigned int k; /* the number of clusters in the kdtree */
	double * data; /* a pointer to the data array */
	int * cluster_size; /* an array showing the size of each cluster */
	int * cluster_start; /* an array of pointers(or indices) to the starting data point of each cluster */
	double ** cluster_bdry; /* an array ([k][2 * dim]) showing the boundary axes of each of the k clusters */
	double ** cluster_centroid; /* an array ([k][dim]) showing the centroid of each of the k clusters */
	int * cluster_assign; /* an array [ndata] showing the cluster assignment of each data point */
};


/*Prototypes*/

/*KDTree_Initialization_Functions.c*/
struct kdtree * initialBuild(int , int , double * );

int * allocateAndInitializeZeroInt(int );

double ** allocateAndInitializeZeroDoubleMulti(int , int );

int getK(int);

int kdtree(struct kdtree **, double *, int, int);

void kdtreeDestroy(struct kdtree *);

/*Construction_Functions.c*/

/*int bipartition(
				int dim, 
				int io, 
				int im, 
				double * data, 
				int * cluster_size, 
				double * cluster_bdry, 
				double * cluster_centroid, 
				int * cluster_assign
				); *//*return value is index of next open cluster in cluster arrays of kdtree struct*/

int bipartition(
				struct kdtree *, /*pointer to kdtree struct*/
				int, /*first data point in cluster*/
				int, /*last data point in cluster*/
				int /* level of partitioning */
				); 

void getCentroidAndBoundaries(struct kdtree *, double *, double *, int, int);

void getVariance(struct kdtree *, double *, double *, int, int);

int getMaxVar(double *, int);

int subdividePartition(struct kdtree *, int, int, int, double);

void swapDataPoints(double *, double *, int);

void saveCurrentCluster(struct kdtree *, double *, double *, int, int);

int getClusterNum(int *, int);


/* Search Functions */

int search_kdtree(struct kdtree *, struct stackBase *, double *); /* Returns number of points searched. */

int getRanges(struct kdtree *, double *, double *); /* Returns index of nearest cluster. */

int getNearestPoint(struct kdtree *, struct stackBase *, double *, int); /* Returns number of points searched. */

int getNearestCluster(struct kdtree *, double *, int);


#endif // !