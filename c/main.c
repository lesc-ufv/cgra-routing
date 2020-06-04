#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm/common.h"

int main(/*int argc, char const *argv[]*/)
{
    CGRA cgra;
    MaskVector mask;
    InputEdgesVector input;

    FILE * gridFile = NULL;
    FILE * edgeFile = NULL;
    unsigned int gridSize, inputSize;

    gridFile = fopen("benchmarks/sbcci/grid/verilog_chebyshev.out", "r");
    edgeFile = fopen("benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in", "r");

    fscanf(edgeFile, "%u %u\n\n", &gridSize, &inputSize);

    CGRAInitialize(&cgra, 2, gridSize);
    MaskVectorInitialize(&mask, &cgra, gridFile);
    InputEdgesVectorInitialize(&input, &mask, &cgra, "benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in");
    InputEdgesVectorPrint(&input);

    printf("TRIVIAL=%d\n", CGRAEdgeIsTrivial(&cgra, 5, 9));

    return 0;
}