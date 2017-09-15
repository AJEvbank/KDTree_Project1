#include "myHeader.h"

void printDataArray(double * dataArray, int dim, int ndata)
{
	int i, j;
	for (i = 0; i < ndata; i++)
	{
		int first_dim_of_data_point = dim * i;
		for (j = 0; j < dim; j++)
		{
			printf("%lf, ", dataArray[first_dim_of_data_point + j]);
			/*printf("(%d,%d), ", i, j);*/
		}
		printf("\n");
	}
	return;
}

void displayKDTree(struct kdtree * Tree)
{
	printf("KDTree: \n");
	printf("dim = %d, ndata = %d, k = %d \n", Tree->dim, Tree->ndata, Tree->k);

	printf("Working data array: \n");
	printDataArray(Tree->data, Tree->dim, Tree->ndata);

	printf("cluster_size: \n");
	printArraysInt(Tree->cluster_size, Tree->k, "size of cluster");

	printf("cluster_start: \n");
	printArraysInt(Tree->cluster_start, Tree->k, "start of cluster");

	printf("cluster_bdry: \n");
	printArraysDouble(Tree->cluster_bdry, Tree->k, 2 * Tree->dim, "bdry of cluster");

	printf("cluster_centroid: \n");
	printArraysDouble(Tree->cluster_centroid, Tree->k, Tree->dim, "centroid of cluster");

	printf("cluster_assign: \n");
	printArraysInt(Tree->cluster_assign, Tree->ndata, "cluster of DP");



	return;
}

void printArraysInt(int * ArrayInt, int size, const char * text)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%s %d = %d, \n", text, i, ArrayInt[i]);
	}
	return;
}

void printArraysDouble(double ** ArrayDouble, int size, int dim, const char * text)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		printf("%s %d = ( ", text, i);
		for (j = 0; j < dim; j++)
		{
			if (j != dim - 1)
			{
				printf("%lf,", ArrayDouble[i][j]);
			}
			else
			{
				printf("%lf", ArrayDouble[i][j]);
			}
		}
		if (i != size - 1)
		{
			printf("),\n");
		}
		else
		{
			printf(")\n");
		}
	}
	return;
}

void printArray(double * array, int size)
{
	int i;
	printf("[ ");
	for (i = 0; i < size; i++)
	{
		printf("%lf ", array[i]);
	}
	printf("]\n");
}

void printStack(struct stackBase *stack)
{
	struct stackNode * iterator = stack->firstNode;
	int i;
	printf("Stack: \n");
	printf("stackDepth = %d, firstNode = %p \n", stack->stackDepth,(void *)stack->firstNode);
	while (iterator != NULL)
	{
		for (i = 0; i < stack->arraySize; i++)
		{
			printf("dim: %d = %lf, \n", i, (iterator->pointArray)[i]);
		}
		printf("distance = %lf, cluster = %d ", iterator->distance, iterator->cluster);
		printf("\n\n");
		iterator = iterator->nextNode;
	}
}

