# Kosaraju Server

This project implements a server that allows connecting several clients, creating graphs, modify them and execute the Kosaraju-Sharir alogrithm on a created graph.

## Table of Contents

- [Kosaraju Server](#kosaraju-server)
  - [Table of Contents](#table-of-contents)
  - [Ex1 - Ex4: Implementing multiuser graph](#ex1---ex4-implementing-multiuser-graph)
    - [Ex1 - Basic Kosaraju implementation](#ex1---basic-kosaraju-implementation)
    - [Ex2 - Profiling](#ex2---profiling)
    - [Ex3 - Interaction with clients](#ex3---interaction-with-clients)
    - [Ex4 - Multi User Server](#ex4---multi-user-server)
  - [Ex5 - Ex6: Implementing Reactor design pattern](#ex5---ex6-implementing-reactor-design-pattern)
    - [Ex5 - Implementing Reactor pattern](#ex5---implementing-reactor-pattern)
    - [Ex6 - Integrating the Reactor pattern](#ex6---integrating-the-reactor-pattern)
  - [Ex7: Implementing Threaded server](#ex7-implementing-threaded-server)
  - [Ex8 - Ex9: Implementing Proactor design pattern](#ex8---ex9-implementing-proactor-design-pattern)
    - [Ex8 - Implementing Proactor pattern](#ex8---implementing-proactor-pattern)
    - [Ex9 - Integrating the Proactor pattern](#ex9---integrating-the-proactor-pattern)
  - [Ex10: Implementing Producer/Consumer concept](#ex10-implementing-producerconsumer-concept)
  - [How to use](#how-to-use)
  - [Execution](#execution)
  - [Authors](#authors)

## Ex1 - Ex4: Implementing multiuser graph

In those levels, we've implemented a multiuser server, that allows multiple connections to occur at the same time, and can recive actions from various users.

The entire code for those levels can be found in `profiling` and `standard` folders.

### Ex1 - Basic Kosaraju implementation

In this level, we've implemented a basic program that allows us to create a graph (given a size), enter which edged exist in the graph, calculates and prints the output of the algorithm.

### Ex2 - Profiling

In this level, we were asked to determine which data structure was the best for holding a graph, and which data structure is the best for implementing the algorithm.
After profiling and comapring the resultes, we came to the conclution that the best data structure for holding a graph a adjecny matrix, and the best data structure for implementing the algorithm is a deque.

For all comparisons and profiling file, check the `profiling` folder.

### Ex3 - Interaction with clients

In this level, we've added interaction with clients through `stdin`.
Now the server support the following commands:

```c
newgraph n,m
kosaraju
newedge i,j
removeedge i,j
```

### Ex4 - Multi User Server

In this level, we've made the server available for multiple users, based on Beej's chat model.

## Ex5 - Ex6: Implementing Reactor design pattern

### Ex5 - Implementing Reactor pattern

In this level, we've implemented the `Reactor` design pattern, using `EventHandler` structs and a list of them in order to manage the connections.

### Ex6 - Integrating the Reactor pattern

In this level, we've integrated the pattern into the basic server from Ex4.

## Ex7: Implementing Threaded server

In this level, we've changed the basic server from Ex4 so now every client has it's own thread. whenever we `accept` a new client, we're opening a new thread for him.

## Ex8 - Ex9: Implementing Proactor design pattern

### Ex8 - Implementing Proactor pattern

In this level, we've implemented the `Proactor` design pattern, using `EventHandler` structs and a list of them in order to manage the connections.

### Ex9 - Integrating the Proactor pattern

In this level, we've integrated the pattern into the basic server from Ex7.

## Ex10: Implementing Producer/Consumer concept

In this level, we've used the `producer/consumer` method in order to create a thread that will indicate the server if 50% of the graph is in the same SCC or not, after a `kosaraju` command was executed. this thread uses `POSIX cond`.

## How to use

For every server (each version is in a different directory) the commands are:

```c
newgraph n,m
kosaraju
newedge i,j
removeedge i,j
```

## Execution

to execute all the servers, use `make`. that will create a different file for every directory.

## Authors

- [Noam Leshem](https://github.com/noamleshem)
- [Eylon Yaakov Katan](https://github.com/eylonk14)
