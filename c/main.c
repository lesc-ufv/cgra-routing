#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm/common.h"
#include "fsm/SimpleWriteOnExec.h"

int main(int argc, char const *argv[])
{
    CGRA cgra;
    MaskVector mask;
    InputEdgesVector input;
    unsigned int grid_count = 0;
    unsigned int nodes, edges;

    FILE * gridFile = NULL;
    FILE * edgeFile = NULL;
    FILE * outputFile = NULL;

    if(argc<=3)
    {
        return EXIT_FAILURE;
    }

    gridFile = fopen(argv[1], "r");
    edgeFile = fopen(argv[2], "r");
    outputFile = fopen(argv[3], "w");

    fscanf(edgeFile, "%u %u", &nodes, &edges);

    fprintf(outputFile, "nodes, edges, size, empty, trivial, nontrivial, routed, bl, cycles, usage, cpu_time, verilog_time\n");

    while (!feof(gridFile))
    {
        if (grid_count==100)
        {
            break;
        }
        grid_count++;

        fprintf(outputFile, "%u,%u,", nodes, edges);

        CGRAInitialize(&cgra, 2, argv[1]);
        MaskVectorInitialize(&mask, &cgra, gridFile, outputFile);
        InputEdgesVectorInitialize(&input, &mask, &cgra, argv[2], outputFile);

        FSM_SimpleWriteOnExec(&cgra, &input, outputFile);

        CGRAFree(&cgra);
        MaskVectorFree(&mask);
        InputEdgesVectorFree(&input);
        fprintf(outputFile, "\n");
    }

    fclose(gridFile);
    fclose(edgeFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}