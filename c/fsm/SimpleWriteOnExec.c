#include "SimpleWriteOnExec.h"

void FSM_SimpleWriteOnExec(CGRA * out_grid, InputEdgesVector * out_input, FILE * out_output)
{
    unsigned int * input = malloc(2*out_input->inputQnt*sizeof(unsigned int));
    bool * grid = malloc(orientation_qnt*out_grid->gridSize*sizeof(bool));
    unsigned int * bypass = malloc(out_grid->gridSize*sizeof(unsigned int));
    unsigned int max_bypass = out_grid->maxBypass;

    InputEdgesVectorCopy(out_input, input);
    CGRAGridCopy(out_grid, grid);
    CGRABypassCopy(out_grid, bypass);

    printf("%u %u\n", input[0], input[1]);
}