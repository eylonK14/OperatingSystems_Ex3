#include "deque.h"

// Function to create a stack of given capacity
Stack *createStack(int capacity)
{
	Stack *stack = (Stack *)malloc(sizeof(Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (int *)malloc(stack->capacity * sizeof(int));
	return stack;
}

// Stack is full when top is equal to the last index
int isFull(Stack *stack)
{
	return stack->top == stack->capacity - 1;
}

// Stack is empty when top is -1
int isEmpty(Stack *stack)
{
	return stack->top == -1;
}

// Function to add an item to stack. It increases top by 1
void push(Stack *stack, int item)
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = item;
}

// Function to remove an item from stack. It decreases top by 1
int pop(Stack *stack)
{
	if (isEmpty(stack))
		return -1;
	return stack->array[stack->top--];
}

// Function to get the transpose of the graph
Graph *transposeGraph(Graph *graph)
{
	Graph *transposed = createGraph(graph->V);
	for (int v = 0; v < graph->V; v++)
	{
#ifdef ADJ_MATRIX
		for (int i = 0; i < graph->V; i++)
		{
			if (graph->G[v][i])
				addEdge(transposed, i, v);
		}
#endif // ADJ_MATRIX

#ifdef ADJ_LIST
		AdjListNode *adjNode = graph->array[v].head;
		while (adjNode != NULL)
		{
			int neighbor = adjNode->dest;
			addEdge(transposed, neighbor, v);
			adjNode = adjNode->next;
		}
#endif // ADJ_LIST
	}
	return transposed;
}

// A recursive function to do DFS starting from v
void DFS(Graph *graph, int v, int visited[], Stack *stack)
{
	// this is made using adjacency matrix
	visited[v] = 1;
#ifdef ADJ_MATRIX
	for (int i = 0; i < graph->V; i++)
	{
		if (graph->G[v][i] && !visited[i])
			DFS(graph, i, visited, stack);
	}
#endif // ADJ_MATRIX

#ifdef ADJ_LIST
	AdjListNode *adjNode = graph->array[v].head;
	while (adjNode != NULL)
	{
		int neighbor = adjNode->dest;
		if (!visited[neighbor])
			DFS(graph, neighbor, visited, stack);
		adjNode = adjNode->next;
	}
#endif // ADJ_LIST

	push(stack, v);
}

// A recursive function to do DFS for the transposed graph
void dfsUtil(Graph *graph, int v, int visited[], int *component, int *index)
{
	visited[v] = 1;
	component[(*index)++] = v;
#ifdef ADJ_MATRIX
	for (int i = 0; i < graph->V; i++)
	{
		if (graph->G[v][i] && !visited[i])
			dfsUtil(graph, i, visited, component, index);
	}
#endif // ADJ_MATRIX

#ifdef ADJ_LIST
	AdjListNode *adjNode = graph->array[v].head;
	while (adjNode != NULL)
	{
		int neighbor = adjNode->dest;
		if (!visited[neighbor])
			dfsUtil(graph, neighbor, visited, component, index);
		adjNode = adjNode->next;
	}
#endif // ADJ_LIST
}

// Function to find and print all strongly connected components
char* kosarajuSCC(Graph *graph)
{

	Stack *stack = createStack(graph->V);
	int *visited = (int *)calloc(graph->V, sizeof(int));

	for (int i = 0; i < graph->V; i++)
	{
		if (!visited[i])
			DFS(graph, i, visited, stack);
	}

	Graph *transposed = transposeGraph(graph);

	for (int i = 0; i < graph->V; i++)
		visited[i] = 0;

	char* result = (char*)malloc(sizeof(char) * graph->V * 100);
	strcpy(result, "");

	while (!isEmpty(stack))
	{
		int v = pop(stack);

		if (!visited[v])
		{
			int *component = (int *)malloc(graph->V * sizeof(int));
			int index = 0;
			dfsUtil(transposed, v, visited, component, &index);

			char temp[100];
			sprintf(temp, "Strongly Connected Component: ");
			strcat(result, temp);

			for (int i = 0; i < index; i++)
			{
				sprintf(temp, "%d ", component[i] + 1);
				strcat(result, temp);
			}

			sprintf(temp, "\n");
			strcat(result, temp);

			free(component);
		}
	}

	free(visited);
	free(stack->array);
	free(stack);
#ifdef ADJ_MATRIX
	for (int i = 0; i < transposed->V; i++)
		free(transposed->G[i]);
	free(transposed->G);
#endif // ADJ_MATRIX

#ifdef ADJ_LIST
	// free(graph->array);
	free(transposed->array);
#endif // ADJ_LIST
	// free(graph);
	free(transposed);

	return result;
}