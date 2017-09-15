#ifndef STACKS

#define STACKS

#include "myHeader.h"

struct stackBase {
	int stackDepth;
	int arraySize;
	struct stackNode * firstNode;
};

struct stackNode {
	double * pointArray;
	double distance;
	int cluster;
	struct stackNode * nextNode;
};


struct stackBase * initStack(int);

void pushNode(double *, double, int,  struct stackBase *);

void pop(struct stackBase *);

int peekDepth(struct stackBase * );

void clearStack(struct stackBase *);

#endif