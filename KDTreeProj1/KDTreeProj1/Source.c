#include "myHeader.h"


int main(int argc, const char * argv[])
{
	srand(4);
	
	int i;
	unsigned int j, len;
	for (i = 0; i < argc; i++)
	{
		len = strlen(argv[i]);
		for (j = 0; j < len; j++)
		{
			printf("%c",argv[i][j]);
		}
		printf(", ");
	}
	printf("\n");

	runMachine(argc, argv);

	
	printf("\n");
	printf("Defaults: dim = %d, ndata = %d, RAND_MAX = %d, max_double = %lf \n", DIM,NDATA,RAND_MAX,MAX_DOUBLE);
	printf("\n");
	system("pause");
	return 0;
}