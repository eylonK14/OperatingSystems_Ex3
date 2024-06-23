#pragma once

#include <stdio.h>
#include <stdlib.h>

// Define the structure for a graph
typedef struct Graph
{
    int V;
    int **G;
} Graph;

// Function to create a graph of V vertices
Graph *createGraph(int);

// Function to add an edge to an undirected graph
void addEdge(Graph *, int, int);