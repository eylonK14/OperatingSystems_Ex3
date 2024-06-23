#include <stdio.h>
#include <stdlib.h>

// #include "adjlist.h"
#include "adjmatrix.h"

// DFS function to reach the destination
int dfs(int curr, int des, Graph* graph, int* vis) {
    if (curr == des) {
        return 1;
    }
    vis[curr] = 1;

    // AdjListNode* temp = graph->array[curr].head;
    // while (temp) {
    //     int x = temp->dest;
    //     if (!vis[x]) {
    //         if (dfs(x, des, graph, vis)) {
    //             return 1;
    //         }
    //     }
    //     temp = temp->next;
    // }

    for (int i = 0; i < graph->V; i++) {
        if (graph->G[curr][i] && !vis[i]) {
            if (dfs(i, des, graph, vis)) {
                return 1;
            }
        }
    }

    return 0;
}

// Function to check whether there is a path from source to destination
int isPath(int src, int des, Graph* graph) {
    int* vis = (int*)calloc(graph->V, sizeof(int));
    int result = dfs(src, des, graph, vis);
    free(vis);
    return result;
}

// Function to return all the strongly connected components of a graph
int** findSCC(int n, int **a, int edges, int* sccSizes, int* numSCCs) {
    int** result = (int**)malloc(n * sizeof(int*));
    int* is_scc = (int*)calloc(n, sizeof(int));

    Graph* graph = createGraph(n);
    for (int i = 0; i < edges; i++) {
        addEdge(graph, a[i][0], a[i][1]);
    }

    int idx = 0;
    for (int i = 0; i < n; i++) {
        if (!is_scc[i]) {
            result[idx] = (int*)malloc(n * sizeof(int));
            int count = 0;

            result[idx][count++] = i;
            for (int j = i + 1; j < n; j++) {
                if (!is_scc[j] && isPath(i, j, graph) && isPath(j, i, graph)) {
                    is_scc[j] = 1;
                    result[idx][count++] = j;
                }
            }

            sccSizes[idx] = count;
            result[idx] = realloc(result[idx], count * sizeof(int));
            idx++;
        }
    }

    *numSCCs = idx;
    free(is_scc);
    free(graph->G);
    //free(graph->array);
    free(graph);

    return result;
}

int **newGraph(int vertices, int edges)
{
	int i;
	int **graph = malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++)
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
	int **graph = newGraph(n, m);

	int* sccSizes = (int*)malloc(n * sizeof(int));
    int numSCCs;
    int** result = findSCC(n, graph, m, sccSizes, &numSCCs);

    for (int i = 0; i < numSCCs; i++) {
        for (int j = 0; j < sccSizes[i]; j++) {
            printf("%d ", result[i][j] + 1);
        }
        printf("\n");
        free(result[i]);
    }

    free(result);
    free(sccSizes);

	return 0;
}
