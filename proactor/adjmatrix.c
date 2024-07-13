#include "adjmatrix.h"

Graph *createGraph(int G)
{
	Graph *graph = (Graph *)malloc(G * sizeof(int));
	graph->V = G;
	graph->G = (int **)malloc(G * sizeof(int *));
	for (int i = 0; i < G; i++)
		graph->G[i] = (int *)calloc(G, sizeof(int));
	printf("Graph created\n");
	for (int i = 0; i < G; i++)
	{
		for (int j = 0; j < G; j++)
		{
			printf("%d ", graph->G[i][j]);
		}
		printf("\n");
	}
	
	return graph;
}

// Function to add an edge to an undirected graph
void addEdge(Graph *graph, int a, int b)
{
	graph->G[a][b] = 1;
}