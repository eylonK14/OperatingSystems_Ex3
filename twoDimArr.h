#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "prof.h"

#ifdef ADJ_LIST
#include "adjlist.h"
#endif // ADJ_LIST

#ifdef ADJ_MATRIX
#include "adjmatrix.h"
#endif // ADJ_MATRIX


int dfs(int, int, Graph *, int *);

int isPath(int, int, Graph *);

void findSCC(Graph *);