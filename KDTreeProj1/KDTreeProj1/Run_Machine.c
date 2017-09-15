#include "myHeader.h"

void runMachine(int argc, const char * argv[])
{
	int dim, ndata,i,j;
	double max_double;
	double * query;
	if (argc >= 2)
	{
		if (isNumber(argv[1])) 
		{
			dim = (int)atof(argv[1]);
		}
	}
	else
	{
		dim = DIM;
	}
	if (argc >= 3)
	{
		if (isNumber(argv[2]))
		{
			ndata = (int)atof(argv[2]);
		}
	}
	else
	{
		ndata = NDATA;
	}
	if (argc >= 4)
	{
		if (isNumber(argv[3]))
		{
			max_double = atof(argv[3]);
		}
	}
	else
	{
		max_double = MAX_DOUBLE;
	}
	query = (double *)malloc(sizeof(double)*dim);
	if (QUERY_ANALYSIS4) { printf("dim = %d, ndata = %d, max_double = %lf \n",dim,ndata,max_double); printf("argc = %d \n", argc); }
	if (argc >= 5)
	{
		for (i = 0, j = 4; i < dim && j < argc; i++, j++)
		{
			if (QUERY_ANALYSIS4) { printf("for loop check \n"); }
			if (isNumber(argv[j]))
			{
				query[i] = (double)atof(argv[j]);
				if (QUERY_ANALYSIS4) { printf("query[%d] = %lf, ", i, query[i]); }
			}
			else
			{
				query[i] = (((double)rand() / (double)(RAND_MAX)) * (max_double)) * signGenerator();
				if (QUERY_ANALYSIS4) { printf("query[%d] = %lf, ", i, query[i]); }
			}
		}
	}
	else
	{
		for (i = 0; i < dim; i++)
		{			
			query[i] = (((double)rand() / (double)(RAND_MAX)) * (max_double)) * signGenerator();	
			if (QUERY_ANALYSIS4) { printf("query[%d] = %lf, ", i, query[i]); }
		}
	}
	if (QUERY_ANALYSIS4) { printArray(query, dim); }

	double * dataArray = dataArrayGenerator(dim, ndata, max_double);

	struct kdtree * Tree = NULL;

	kdtree(&Tree, dataArray, dim, ndata);

	struct stackBase * result = initStack(Tree->dim);

	int pointCount = search_kdtree(Tree, result, query);

	writeResults(dim, ndata, Tree->data, Tree->cluster_assign);

	if (FINAL_RESULT)
	{
		printf("Query Point: \n");
		printArray(query, dim);
		printf("Number of points searched = %d \n", pointCount);
		printf("Nearest Point(s): \n");
		int n = 1;
		struct stackNode * iterator = result->firstNode;
		while (iterator != NULL)
		{
			printf("Point: %d:%d \n", n, result->stackDepth);
			n++;
			printf("[ ");
			for (i = 0; i < result->arraySize; i++)
			{
				printf("%lf ", (iterator->pointArray)[i]);
			}
			printf(" ] \n");
			printf("distance = %lf, cluster = %d \n", iterator->distance, iterator->cluster);
			iterator = iterator->nextNode;
		}

		printf("KDTree: ****************************************************************************************************************\n");
		displayKDTree(Tree);
	}

	free(query);
	clearStack(result);
	kdtreeDestroy(Tree);
	return;
}

int isNumber(const char * str)
{
	int i;
	int len = strlen(str);
	char ch;
	enum isNumStates state = INITIAL;
	if (len > 0)
	{
		for (i = 0; i < len; i++)
		{
			ch = str[i];
			switch (state)
			{
			case INITIAL:
				{
					if (ch == '-' || ch == '+')
					{
						state = PLUSORMINUS;
					}
					else if (ch == '0')
					{
						state = ZERO;
					}
					else if (isdigit(ch))
					{
						state = NUMBER;
					}
					else
					{
						state = ERROR;
					}
					break;
				}

			case PLUSORMINUS:
				{
					if (ch == '0')
					{
						state = ZERO;
					}
					else if (isdigit(ch))
					{
						state = NUMBER;
					}
					else
					{
						state = ERROR;
					}
					break;
				}

			case ZERO:
				{
					if (ch == '.')
					{
						state = DECIMAL;
					}
					else 
					{
						state = ERROR;
					}
					break;
				}

			case NUMBER:
				{
					if (isdigit(ch))
					{
						state = NUMBER;
					}
					else if (ch == '.')
					{
						state = DECIMAL;
					}
					else
					{
						state = ERROR;
					}
					break;
				}

			case DECIMAL:
				{
					if (isdigit(ch))
					{
						state = DECIMAL;
					}
					else
					{
						state = ERROR;
					}
					break;
				}

			case ERROR:
				{
					return 0;
					break;
				}

			default:
			{
				printf("default in isNumber: %d \n", (int)state);
				return 0;
				break;
			}

			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

void writeResults(int dim, int ndata, double* data, int* cluster_assign)
{
	int i;
	FILE* file;
	fopen_s(&file, "data.txt", "w");

	fprintf_s(file, "%d\n", dim);
	fprintf_s(file, "%d\n", ndata);
	for (i = 0; i < dim * ndata; i++)
		fprintf_s(file, "%lf\n", data[i]);

	for (i = 0; i < ndata; i++)
		fprintf_s(file, "%d\n", cluster_assign[i]);

	fclose(file);
}