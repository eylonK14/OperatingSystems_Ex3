#pragma once

#include "prof.h"

#ifdef ADJ_LIST
#include "adjlist.h"
#endif // ADJ_LIST

#ifdef ADJ_MATRIX
#include "adjmatrix.h"
#endif // ADJ_MATRIX

// A structure to represent a stack
typedef struct Stack {
    int top;
    int capacity;
    int* array;
} Stack;

Stack *createStack(int);

int isFull(Stack *);

int isEmpty(Stack *);

void push(Stack *, int);

int pop(Stack *);

void DFS(Graph *, int, int[], Stack *);

Graph *transposeGraph(Graph *);

void dfsUtil(Graph *, int v, int[], int *, int *);

char* kosarajuSCC(Graph *);