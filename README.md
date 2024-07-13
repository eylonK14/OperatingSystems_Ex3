# Kosaraju Server

This project implements a server that allows connecting several clients, creating graphs, modify them and execute the Kosaraju-Sharir alogrithm on a created graph.

## Ex1 - Ex4: Implementing multiuser graph

In those levels, we've implemented a multiuser server, that allows multiple connections to occur at the same time, and can recive actions from various users.

The entire code for those levels can be found in `profiling` and `standard` folders.

### Ex1 - Basic Kosaraju implementation

In this level, we've implemented a basic program that allows us to create a graph (given a size), enter which edged exist in the graph, calculates and prints the output of the algorithm.

### Ex2 - Profiling

In this level, we were asked to determine which data structure was the best for holding a graph, and which data structure is the best for implementing the algorithm.
After profiling and comapring the resultes, we came to the conclution that the best data structure for holding a graph a adjecny matrix, and the best data structure for implementing the algorithm is a deque.

For all comparisons and profiling file, check the `profiling` folder.
