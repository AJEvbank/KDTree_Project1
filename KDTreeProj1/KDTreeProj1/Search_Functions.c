#include "myHeader.h"

int search_kdtree(struct kdtree * Tree, struct stackBase * result, double * query)
{
	double * clusterRanges = (double *)malloc(sizeof(double)*Tree->k);
	int clusterCount = 0, loopControl;
	unsigned int i,k = Tree->k;
	double minDist;
	int nearestCluster = getRanges(Tree, clusterRanges, query);
	if (QUERY_ANALYSIS3) { printArray(clusterRanges, k); }
	do
	{
		clusterCount += getNearestPoint(Tree,result,query,nearestCluster);
		minDist = (result->firstNode)->distance;
		clusterRanges[nearestCluster] = INFINITY;
		loopControl = 0;
		for (i = 0; i < Tree->k; i++)
		{
			if (clusterRanges[i] <= minDist)
			{
				if (QUERY_ANALYSIS3) { printf("clusterRanges[%d] = %lf, \n", i, clusterRanges[i]); }
				nearestCluster = i;
				loopControl = 1;
				break;
			}
		}		
	} while (loopControl);

	free(clusterRanges);
	return clusterCount;
}

int getRanges(struct kdtree * Tree, double * clusterRanges, double * query)
{
	unsigned int i;
	int j;
	int minCluster;
	double ** clusters = Tree->cluster_bdry;

	for (i = 0; i < Tree->k; i++)
	{
		clusterRanges[i] = 0;
		for (j = 0; j < Tree->dim; j++)
		{
			int first_index_of_dim = j * 2;
			if (query[j] < clusters[i][first_index_of_dim])
			{
				clusterRanges[i] += pow( fabs( (query[j] - clusters[i][first_index_of_dim]) ), 2);
			}
			else if (query[j] > clusters[i][first_index_of_dim+1])
			{
				clusterRanges[i] += pow( fabs( (query[j] - clusters[i][first_index_of_dim +1]) ), 2);
			}
			else
			{
				;
			}			
		}
		if (QUERY_ANALYSIS3) { printf("clusterRanges[%d] = %lf, \n", i, clusterRanges[i]); }
	}
	minCluster = 0;
	for (i = 0; i < Tree->k; i++)
	{
		clusterRanges[i] = sqrt(clusterRanges[i]);
		if (clusterRanges[i] < clusterRanges[minCluster])
		{
			minCluster = i;
		}
	}
	return minCluster;
}

int getNearestPoint(struct kdtree * Tree, struct stackBase * result, double * query, int nearestCluster)
{
	double * thisPoint = (double *)malloc(sizeof(double)*Tree->dim);
	int i,j,k, dataPoint,start = (Tree->cluster_start)[nearestCluster],size = (Tree->cluster_size)[nearestCluster];
	double distanceCalculating;
	if (QUERY_ANALYSIS2) { printf("(Tree->cluster_start)[nearestCluster] = %d,\n", start); 
	printf("(Tree->cluster_size)[nearestCluster] = %d,\n", size);
	}
	for (i = start; i < start + size; i++)
	{
		dataPoint = i * Tree->dim;
		distanceCalculating = 0;
		if (QUERY_ANALYSIS2) { printf("dataPoint = %d \n", dataPoint); }
		for (j = 0; j < Tree->dim; j++)
		{
			distanceCalculating += pow( fabs((query[j] - (Tree->data)[dataPoint+j])), 2);
			if (QUERY_ANALYSIS2) { printf("with data[%d] -> %lf \n",(dataPoint+j), distanceCalculating); }
		}
		distanceCalculating = sqrt(distanceCalculating);
		if (QUERY_ANALYSIS2) { printf("final = %lf \n", distanceCalculating); }
		if (result->stackDepth == 0) {
			for (k = 0 ; k < Tree->dim; k++) { thisPoint[k] = (Tree->data)[dataPoint + k]; }
			if (QUERY_ANALYSIS) { printArray(thisPoint, Tree->dim); }
			pushNode(thisPoint, distanceCalculating, nearestCluster, result);
			if (QUERY_ANALYSIS) { printStack(result); }
		}
		else
		{
			if (distanceCalculating < (result->firstNode)->distance)
			{
				for (k = 0; k < Tree->dim; k++) { thisPoint[k] = (Tree->data)[dataPoint + k]; }
				if (QUERY_ANALYSIS) { printArray(thisPoint, Tree->dim); }
				clearStack(result);
				pushNode(thisPoint, distanceCalculating, nearestCluster, result);
			}
			else if (distanceCalculating == (result->firstNode)->distance)
			{
				for (k = 0; k < Tree->dim; k++) { thisPoint[k] = (Tree->data)[dataPoint + k]; }
				if (QUERY_ANALYSIS) { printArray(thisPoint, Tree->dim); }
				pushNode(thisPoint, distanceCalculating, nearestCluster, result);
			}
		}
	}
	if (QUERY_ANALYSIS) { printStack(result); }
	free(thisPoint);
	return (Tree->cluster_size)[nearestCluster];
}

int getNearestCluster(struct kdtree * Tree, double * clusterRanges, int nearestCluster)
{
	unsigned int i;
	int nextNearestCluster = nearestCluster;
	for (i = 0; i < Tree->k; i++)
	{
		if (clusterRanges[i] <= clusterRanges[nextNearestCluster])
		{
			nextNearestCluster = i;
		}
	}
	return nextNearestCluster;
}


