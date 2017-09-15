#include "myHeader.h"

double * dataArrayGenerator(int dim, int ndata, double max_double)
{
	int i = 0;

	double * dataArray = (double *)malloc(dim * ndata * sizeof(double));

	for (i = 0; i < ndata; i++)
	{
		int first_dim_of_data_point = dim * i;
		int j = 0;
		for (j = 0; j < dim; j++)
		{
			dataArray[first_dim_of_data_point + j] = (((double)rand()/(double)(RAND_MAX)) * (max_double)) * signGenerator();
			/*printf("dataArray[%d] = %lf, ", (first_dim_of_data_point + j),dataArray[first_dim_of_data_point + j]);*/
		}	
		/*printf("\n");*/
	}

	return dataArray;
}

int signGenerator()
{	
	int sign = rand() % 2;
	if (sign == 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}


