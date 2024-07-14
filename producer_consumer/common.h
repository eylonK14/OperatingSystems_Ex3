#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct _Graph Graph;

// Function to create a graph of V vertices
Graph *createGraph(int);

// Function to add an edge to an undirected graph
void addEdge(Graph *, int, int);