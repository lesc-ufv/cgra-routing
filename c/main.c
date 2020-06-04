#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm/common.h"
#include "fsm/SimpleWriteOnExec.h"

int main(/*int argc, char const *argv[]*/)
{
    CGRA cgra;
    MaskVector mask;
    InputEdgesVector input;

    FILE * gridFile = NULL;
    FILE * edgeFile = NULL;
    FILE * outputFile = NULL;
    unsigned int gridSize, inputSize;

    gridFile = fopen("benchmarks/sbcci/grid/verilog_chebyshev.out", "r");
    edgeFile = fopen("benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in", "r");
    outputFile = fopen("output/sbcci/chebyshev_0.csv", "w");

    fscanf(edgeFile, "%u %u\n\n", &gridSize, &inputSize);

    fclose(edgeFile);

    fprintf(outputFile, "trivial, nontrivial, cycles, bl, routed, usage\n");

    while (!feof(gridFile))
    {
        CGRAInitialize(&cgra, 2, gridSize);
        MaskVectorInitialize(&mask, &cgra, gridFile);
        InputEdgesVectorInitialize(&input, &mask, &cgra, "benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in", outputFile);

        FSM_SimpleWriteOnExec(&cgra, &input, outputFile);

        CGRAFree(&cgra);
        MaskVectorFree(&mask);
        InputEdgesVectorFree(&input);
        fprintf(outputFile, "\n");
    }

    fclose(gridFile);
    fclose(outputFile);

    return 0;
}