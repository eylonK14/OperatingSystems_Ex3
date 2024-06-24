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

int words(char sentence[])
{
    int count = 1;
    for (int i = 0; sentence[i] != '\0'; ++i)
        if (sentence[i] == ' ')
            count++;
    return count;
}

void parse(char *input, Graph **graph)
{
    int word_count = words(input);
    if (word_count > 1)
    {
        char **command = (char **)malloc(sizeof(char *) * (word_count + 1));
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL)
        {
            command[i] = (char *)malloc(sizeof(char) * strlen(token));
            command[i++] = token;
            token = strtok(NULL, " ");
        }
        command[word_count] = NULL;
        if (!strcmp(command[0], "newgraph"))
        {
            int n = command[1][0] - '0', m = command[1][2] - '0';
            int **myGraph = newGraph(m);

            *graph = createGraph(n);
            for (int i = 0; i < m; i++)
            {
                addEdge(*graph, myGraph[i][0], myGraph[i][1]);
            }
        }
        else if (!strcmp(command[0], "newedge"))
        {
            addEdge(*graph, command[1][0] - '1', command[1][2] - '1');
        }
        else if (!strcmp(command[0], "removeedge"))
        {
            (*graph)->G[command[1][0] - '1'][command[1][2] - '1'] = 0;
        }
    }
    else if (!strcmp(input, "exit"))
    {
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp(input, "kosaraju"))
    {
        kosarajuSCC(*graph);
    }
}

char *myGetLine(char *data)
{
    data = NULL;
    size_t len = 0;
    int ret = 0;
    while (ret < 2)
        ret = getline(&data, &len, stdin);
    data[strcspn(data, "\r\n")] = 0;
    return data;
}

int main()
{
    printf("welcome to our graph factory what would you like to do?\n");
    printf("possible options are:\n\tnewgraph i,j\n\tkosaraju\n\tnewedge i,j\n\tremoveedge i,j\n\texit\n");

    Graph *myGraph = NULL;
    while (1)
    {
        char *input = NULL;
        input = myGetLine(input);
        parse(input, &myGraph);
    }

#ifdef TWO_DIM_ARR
    findSCC(newGraph);
#endif // TWO_DIM_ARR

#ifdef DEQUE
    kosarajuSCC(myGraph);
#endif // DEQUE

    return 0;
}
