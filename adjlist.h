#pragma once

#include <stdio.h>
#include <stdlib.h>

// Define the structure for an adjacency list node
typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

// Define the structure for an adjacency list
typedef struct AdjList {
    AdjListNode* head;
} AdjList;

// Define the structure for a graph
typedef struct Graph {
    int V;
    AdjList* array;
} Graph;

// Function to create a new adjacency list node
AdjListNode* newAdjListNode(int);

// Function to create a graph of V vertices
Graph* createGraph(int);

// Function to add an edge to an undirected graph
void addEdge(Graph*, int, int);