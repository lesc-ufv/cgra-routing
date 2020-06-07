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

    FILE * gridFile = NULL;
    FILE * outputFile = NULL;

    if(argc<=3)
    {
        return EXIT_FAILURE;
    }

    gridFile = fopen(argv[1], "r");
    outputFile = fopen(argv[3], "w");

    fprintf(outputFile, "trivial, nontrivial, cycles, bl, routed, usage\n");

    while (!feof(gridFile))
    {
        CGRAInitialize(&cgra, 2, argv[1]);
        MaskVectorInitialize(&mask, &cgra, gridFile);
        InputEdgesVectorInitialize(&input, &mask, &cgra, argv[2], outputFile);

        FSM_SimpleWriteOnExec(&cgra, &input, outputFile);

        CGRAFree(&cgra);
        MaskVectorFree(&mask);
        InputEdgesVectorFree(&input);
        fprintf(outputFile, "\n");
    }

    fclose(gridFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}