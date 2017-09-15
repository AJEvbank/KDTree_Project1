#include "myHeader.h"

struct kdtree * initialBuild(int dim, int ndata, double * data)
{
	struct kdtree * buildTree = (struct kdtree *)malloc(sizeof(struct kdtree));
	buildTree->dim = dim;
	buildTree->ndata = ndata;
	buildTree->k = getK(ndata);
	buildTree->data = data;
	buildTree->cluster_size = allocateAndInitializeZeroInt(buildTree->k);
	buildTree->cluster_start = allocateAndInitializeZeroInt(buildTree->k);
	buildTree->cluster_bdry = allocateAndInitializeZeroDoubleMulti(buildTree->k, 2 * dim);
	buildTree->cluster_centroid = allocateAndInitializeZeroDoubleMulti(buildTree->k, dim);
	buildTree->cluster_assign = allocateAndInitializeZeroInt(ndata);

	return buildTree;
}

int * allocateAndInitializeZeroInt(int size_of_target)
{
	int * target = (int *)malloc(size_of_target * sizeof(int));
	int i = 0;
	for (i = 0; i < size_of_target; i++) {
		target[i] = 0;
		if (DEBUG_INITIALIZATION) printf("%d, ", target[i]);
	}
	if (DEBUG_INITIALIZATION) printf("\n");
	return target;
}

double ** allocateAndInitializeZeroDoubleMulti(int k, int dimension)
{
	double ** target = (double **)malloc(sizeof(double *)* k);
	int i,j;
	for (i = 0; i < k; i++)
	{
		target[i] = (double *)malloc(sizeof(double) * dimension);
		for (j = 0; j < dimension; j++)
		{
			target[i][j] = 0;
			if (DEBUG_INITIALIZATION) printf("(%lf), ", target[i][j]);
		}
		if (DEBUG_INITIALIZATION) printf("\n");
	}
	if (DEBUG_INITIALIZATION) printf("\n");
	return target;
}

int getK(int ndata)
{
	int root = (int)ceil(log2(ceil(sqrt(ndata))));
	return (int)pow(2,root);
}

int kdtree(struct kdtree ** Tree, double * dataArray, int dim, int ndata)
{
	*Tree = initialBuild(dim, ndata, dataArray);
	if (KDTREE_INIT) { printf("Initial kdtree: \n"); displayKDTree(*Tree); };
	bipartition(*Tree, 0, ndata - 1, (*Tree)->k);
	return 0;
}

void kdtreeDestroy(struct kdtree * Tree)
{
	unsigned int i;	
	for (i = 0; i < Tree->k; i++)
	{
		free(Tree->cluster_bdry[i]);
		free(Tree->cluster_centroid[i]);
	}
	free(Tree->cluster_assign);
	free(Tree->cluster_size);
	free(Tree->cluster_start);
	free(Tree->data);
	free(Tree);
}