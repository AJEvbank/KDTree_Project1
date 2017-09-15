#include "myHeader.h"

int bipartition(struct kdtree * Tree, int io, int im, int Klevel)
{
	double * mean = (double *)malloc(sizeof(double)*(Tree->dim));
	double * variance = (double *)malloc(sizeof(double)*(Tree->dim));
	double * boundaries = (double *)malloc(sizeof(double)*2*(Tree->dim));
			
	int i;
	for (i = 0; i < Tree->dim; i++) { mean[i] = 0; variance[i] = 0; }

	/*1. Find centroid of data set as well as min and max of cluster.*/

	getCentroidAndBoundaries(Tree, mean, boundaries, io, im);

		if (DATA_ANALYSIS) printArray(mean, Tree->dim);

	/*2. Compute variance of each dimension.*/

	getVariance(Tree, mean, variance, io, im);

		if (DATA_ANALYSIS) printArray(variance, Tree->dim);

	/*3. Choose the dimension with the largest variance.*/

	int maxVarDim = getMaxVar(variance, Tree->dim);
		
		if (DATA_ANALYSIS) printf("Dimension of Maximum Variance = %d \n", maxVarDim);

	/*4. Partition the data set along the dimension chosen in step 3. */
	
	if (Klevel == 1) 
	{
		/*If the partition is too small to divide, then it should be stored.*/
		saveCurrentCluster(Tree, mean, boundaries, io, im);
		return 0;
	}
	else
	{
		int leftPartition = subdividePartition(Tree, io, im, maxVarDim, mean[maxVarDim]);
		if (PARTITION) printf("leftPartition = %d \n", leftPartition);
		if (PARTITION) printDataArray(Tree->data + (io * Tree->dim),Tree->dim, (im - io + 1));
		

		
		bipartition(Tree, io, leftPartition, Klevel/2);
		bipartition(Tree, leftPartition + 1, im, Klevel/2);		
	}

	free(mean);
	free(boundaries);
	free(variance);
	return 0;
}

void getCentroidAndBoundaries(struct kdtree * Tree, double * meanArray, double * boundaries, int io, int im)
{
	int i, j;
	double divisor = ((double)(im - io + 1));
	double * dataArray = Tree->data;
	for (i = 0; i < Tree->dim; i++) 
	{ 
		boundaries[2*i] = dataArray[(io*Tree->dim) + i];
		boundaries[(2*i)+1] = dataArray[(io*Tree->dim) + i];
	}
	for (i = io; i <= im; i++)
	{
		int first_index_of_dp = i * Tree->dim;
		
		for (j = 0; j < Tree->dim; j++)
		{
			meanArray[j] = meanArray[j] + dataArray[first_index_of_dp + j];
			boundaries[2 * j] = fmin(boundaries[2 * j], dataArray[first_index_of_dp + j]);
			boundaries[(2 * j) + 1] = fmax(boundaries[(2 * j) + 1], dataArray[first_index_of_dp + j]);
		}
	}
	for (i = 0; i < Tree->dim; i++)
	{
		meanArray[i] = meanArray[i] / divisor;
	}
}

void getVariance(struct kdtree * Tree, double * meanArray, double * varianceArray, int io, int im)
{
	int i, j;
	double * dataArray = Tree->data;
	double divisor = (double)(im - io + 1);
	for (i = io; i <= im; i++)
	{
		int first_index_of_dp = i * Tree->dim;
		for (j = 0; j < Tree->dim; j++) {
			varianceArray[j] = varianceArray[j] + pow((dataArray[first_index_of_dp + j] - meanArray[j]), 2);
		}
	}
	for (j = 0; j < Tree->dim; j++) {
		varianceArray[j] = varianceArray[j] / divisor;
	}
}

int getMaxVar(double * varianceArray, int size)
{
	int i,dimension = 0;
	for (i = 0; i < size; i++)
	{
		if (varianceArray[i] > varianceArray[dimension])
		{
			dimension = i;
		}
	}
	return dimension;
}

int subdividePartition(struct kdtree * Tree, int io, int im, int maxVarDim, double meanMaxVarDim)
{
	if (DEBUG) printf("io = %d, im = %d, maxVarDim = %d, meanMaxVarDim = %lf \n",io,im,maxVarDim,meanMaxVarDim);
	double * data = Tree->data;
	int i = (io * Tree->dim) + maxVarDim, m = (im * Tree->dim) + maxVarDim, inc = Tree->dim;
	int left, right, leftTrack;
	if (DEBUG) printf("i = %d, m = %d, inc = %d \n",i,m,inc);
	while(i+inc < m)
	{
		if (DEBUG) printf("iteration: i = %d, m = %d \n", i, m);
		left = (data[i] < meanMaxVarDim);
		right = (data[m] >= meanMaxVarDim);

		if (!left && !right)
		{
			swapDataPoints((data + i) - maxVarDim, (data + m) - maxVarDim, Tree->dim);
			i = i + inc;
			if (i+inc == m) { continue; }
			m = m - inc;
		}
		else if(!left && right)
		{
			m = m - inc;
		}
		else if (left && !right)
		{
			i = i + inc;
		}
		else
		{
			i = i + inc;
			if (i+inc == m) { continue; }
			m = m - inc;
		}		
		leftTrack = i;
	}

	left = (data[i] < meanMaxVarDim);
	right = (data[m] >= meanMaxVarDim);

	if (!left && !right)
	{
		swapDataPoints((data + i) - maxVarDim, (data + m) - maxVarDim, Tree->dim);
	}
	else if (!left && right)
	{
		m = m - inc;
		i = i - inc;
	}
	else if (left && !right)
	{
		i = i + inc;
		m = m + inc;
	}
	leftTrack = i;
	leftTrack = (leftTrack - maxVarDim)/Tree->dim;
	return leftTrack;
}

void swapDataPoints(double * leftSide, double * rightSide, int dim)
{
	int i;
	double temp;
	for (i = 0; i < dim; i++)
	{
		temp = leftSide[i];
		leftSide[i] = rightSide[i];
		rightSide[i] = temp;
	}
}

void saveCurrentCluster(struct kdtree * Tree,double * meanArray, double * boundaries, int io, int im)
{
	int i;
	int clusterNum = getClusterNum(Tree->cluster_size,Tree->k);
	if (clusterNum == -1) { printf("Too many clusters. k = %d \n",Tree->k); system("pause"); }
	(Tree->cluster_size)[clusterNum] = (im - io + 1);
	(Tree->cluster_start)[clusterNum] = io;
	for (i = 0; i < Tree->dim; i++) { (Tree->cluster_centroid)[clusterNum][i] = meanArray[i]; }
	for (i = 0; i < (2 * Tree->dim); i++) { (Tree->cluster_bdry)[clusterNum][i] = boundaries[i]; }
	for (i = io; i <= im; i++) { (Tree->cluster_assign)[i] = clusterNum; }
}

int getClusterNum(int * cluster_size, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (cluster_size[i] == 0)
		{
			return i;
		}
	}
	return -1;
}

//double * getBoundaries(struct kdtree * Tree, int io, int im)
//{
//	int dim = Tree->dim;
//	double * bdryArray = (double *)malloc((2 * dim) * sizeof(double));
//	int i, j;
//	/*initialize bdryArray with the coordinates of io*/
//	for (i = 0; i < Tree->dim; i++)
//	{
//		int track = i * 2;
//		bdryArray[track] = (Tree->data)[i];
//		bdryArray[track+1] = (Tree->data)[i];
//	}
//	for (i = io+1; i <= im; i++)
//	{
//		int first_dim_of_dp = i * Tree->dim;
//		for (j = 0; j < Tree->dim; j++)
//		{
//			if ((Tree->data)[first_dim_of_dp + j] < bdryArray[j]) { bdryArray[j] = (Tree->data)[j]; }
//			if ((Tree->data)[first_dim_of_dp + j] > bdryArray[j+1]) { bdryArray[j+1] = (Tree->data)[j]; }
//		}
//	}
//	return bdryArray;
//}

//void setBoundaryByParent(struct kdtree * Tree, double * boundaries, enum boundaryType * boundaryTypes, struct parent_bdry parentBoundary)
//{
//	int i;
//	if (parentBoundary.maxVarDim == -1)
//	{
//		/* -1 means there is no parent cluster */
//		return;
//	}
//	else
//	{
//		for (i = 0; i < (2 * Tree->dim); i++)
//		{
//			if (parentBoundary.type[i] == MEAN)
//			{
//				boundaries[i] = parentBoundary.boundaries[i];
//				boundaryTypes[i] = parentBoundary.type[i];
//			}			
//		}
//		if (parentBoundary.lastSplitType == UPPER)
//		{
//			boundaries[2 * parentBoundary.maxVarDim] = parentBoundary.lastSplitValue;
//			boundaryTypes[2 * parentBoundary.maxVarDim] = MEAN;
//
//		}
//		else if (parentBoundary.lastSplitType == LOWER)
//		{
//			boundaries[(2 * parentBoundary.maxVarDim) + 1] = parentBoundary.lastSplitValue;
//			boundaryTypes[(2 * parentBoundary.maxVarDim) + 1] = MEAN;
//		}
//	}
//	return;
//}

