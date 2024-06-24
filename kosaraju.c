#include <stdio.h>
#include <stdlib.h>

#include "prof.h"

#ifdef TWO_DIM_ARR
#include "twoDimArr.h"
#endif // TWO_DIM_ARR

#ifdef DEQUE
#include "deque.h"
#endif // DEQUE


int **newGraph(int edges)
{
    int i;
    int **graph = malloc(edges * sizeof(int *));
    for (i = 0; i < edges; i++)
    {
        graph[i] = (int *)calloc(2, sizeof(int));
    }

    for (i = 0; i < edges; i++)
    {
        int a, b;
        printf("Enter the edge #%d: ", i + 1);
        scanf("%d %d", &a, &b);
        graph[i][0] = a - 1;
        graph[i][1] = b - 1;
    }
    return graph;
}

int main()
{
    int n, m;
    printf("Enter the number of vertices and edges: ");
    scanf("%d %d", &n, &m);
    int **graph = newGraph(m);

    Graph *newGraph = createGraph(n);
    for (int i = 0; i < m; i++)
    {
        addEdge(newGraph, graph[i][0], graph[i][1]);
    }

#ifdef TWO_DIM_ARR
    findSCC(newGraph);
#endif // TWO_DIM_ARR

#ifdef DEQUE
    kosarajuSCC(newGraph);
#endif // DEQUE

    return 0;
}
