#include "twoDimArr.h"

// DFS function to reach the destination
int dfs(int curr, int des, Graph *graph, int *vis)
{
	if (curr == des)
	{
		return 1;
	}
	vis[curr] = 1;

#ifdef ADJ_LIST
	AdjListNode *temp = graph->array[curr].head;
	while (temp)
	{
		int x = temp->dest;
		if (!vis[x])
		{
			if (dfs(x, des, graph, vis))
			{
				return 1;
			}
		}
		temp = temp->next;
	}
#endif // ADJ_LIST

#ifdef ADJ_MATRIX
	for (int i = 0; i < graph->V; i++)
	{
		if (graph->G[curr][i] && !vis[i])
		{
			if (dfs(i, des, graph, vis))
			{
				return 1;
			}
		}
	}
#endif // ADJ_MATRIX

	return 0;
}

// Function to check whether there is a path from source to destination
int isPath(int src, int des, Graph *graph)
{
	int *vis = (int *)calloc(graph->V, sizeof(int));
	int result = dfs(src, des, graph, vis);
	free(vis);
	return result;
}

// Function to return all the strongly connected components of a graph
void findSCC(Graph *graph)
{
	int *sccSizes = (int *)malloc(graph->V * sizeof(int));
	int **result = (int **)malloc(graph->V * sizeof(int *));
	int *is_scc = (int *)calloc(graph->V, sizeof(int));

	int idx = 0;
	for (int i = 0; i < graph->V; i++)
	{
		if (!is_scc[i])
		{
			result[idx] = (int *)malloc(graph->V * sizeof(int));
			int count = 0;

			result[idx][count++] = i;
			for (int j = i + 1; j < graph->V; j++)
			{
				if (!is_scc[j] && isPath(i, j, graph) && isPath(j, i, graph))
				{
					is_scc[j] = 1;
					result[idx][count++] = j;
				}
			}

			sccSizes[idx] = count;
			result[idx] = realloc(result[idx], count * sizeof(int));
			idx++;
		}
	}

	for (int i = 0; i < idx; i++)
	{
		for (int j = 0; j < sccSizes[i]; j++)
		{
			printf("%d ", result[i][j] + 1);
		}
		printf("\n");
		free(result[i]);
	}
	free(result);
	free(sccSizes);
	free(is_scc);
#ifdef ADJ_LIST
	free(graph->array);
#endif // ADJ_LIST

#ifdef ADJ_MATRIX
	free(graph->G);
#endif // ADJ_MATRIX
	free(graph);
}