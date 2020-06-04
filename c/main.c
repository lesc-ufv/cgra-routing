#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm/common.h"

int main(/*int argc, char const *argv[]*/)
{
    CGRA cgra;
    MaskVector mask;
    InputEdgesVector input;

    FILE * grid = NULL;
    FILE * edgeList = NULL;
    unsigned int gridSize, inputSize;

    grid = fopen("benchmarks/sbcci/grid/verilog_chebyshev.out", "r");
    edgeList = fopen("benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in", "r");

    fscanf(edgeList, "%u %u\n\n", &gridSize, &inputSize);

    CGRAInitialize(&cgra, 2, gridSize);
    MaskVectorInitialize(&mask, &cgra, grid);
    InputEdgesVectorInitialize(&input, &mask, edgeList, inputSize);
    InputEdgesVectorPrint(&input);

    return 0;
}