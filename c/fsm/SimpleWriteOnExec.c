#include "SimpleWriteOnExec.h"

#define swe_init 0
#define swe_prefetch 1
#define swe_nextedge 2
#define swe_end 3

#define swe_x_test 4
#define swe_xinc_test 5
#define swe_xinc_set 6
#define swe_xdec_test 7
#define swe_xdec_set 8

#define swe_y_test 9
#define swe_yinc_test 10
#define swe_yinc_set 11
#define swe_ydec_test 12
#define swe_ydec_set 13

#define swe_xy_test 14
#define swe_modified_test 15

#define swe_blacklist 16

void FSM_SimpleWriteOnExec(CGRA * out_grid, InputEdgesVector * out_input, FILE * out_output)
{
    // Constants
    unsigned int max_bypass = out_grid->maxBypass;
    unsigned int gridlineSize = floorSqrt(out_grid->gridSize);


    // Memories
    unsigned int * input = malloc(2*out_input->inputQnt*sizeof(unsigned int));
    bool * grid = malloc(orientation_qnt*out_grid->gridSize*sizeof(bool));
    unsigned int * bypass = malloc(out_grid->gridSize*sizeof(unsigned int));
    unsigned int * stackNode = malloc(2*(gridlineSize-1)*sizeof(unsigned int));
    unsigned int * stackOutput = malloc(2*(gridlineSize-1)*sizeof(unsigned int));

    printf("GRID--%d\n", grid[32]);

    InputEdgesVectorCopy(out_input, input);
    CGRAGridCopy(out_grid, grid);
    CGRABypassCopy(out_grid, bypass);

    // Internals
    unsigned int state;
    unsigned int prefetch[2], current[2];
    unsigned int inputIndex, stackIndex;
    bool modified, firstEdge;

    // Next internals
    unsigned int next_state;
    unsigned int next_prefetch[2], next_current[2];
    unsigned int next_inputIndex, next_stackIndex;
    bool next_modified, next_firstEdge;

    // Simulating reset pressed
    state = swe_init;

    while (true)
    {
        switch (state)
        {

        case swe_init:
            printf("swe_init\n");

            next_inputIndex = 2;


            next_state = swe_prefetch;
            break;

        case swe_prefetch:

            next_prefetch[0] = input[0];
            next_prefetch[1] = input[1];

            next_state = swe_nextedge;
            printf("swe_prefetch [%u-%u]\n", next_prefetch[0], next_prefetch[1]);
            break;

        case swe_nextedge:


            if(prefetch[0]==0 && prefetch[1]==0)
            {
                next_state = swe_end;
            }
            else
            {
                next_current[0] = prefetch[0];
                next_current[1] = prefetch[1];

                next_prefetch[0] = input[inputIndex + 0];
                next_prefetch[1] = input[inputIndex + 1];

                next_inputIndex = inputIndex + 2;
                next_stackIndex = 0;
                next_firstEdge = true;
                next_modified = false;

                next_state = swe_x_test;
            }
            printf("swe_nextedge C[%u-%u] P[%u-%u]\n", next_current[0], next_current[1], next_prefetch[0], next_prefetch[1]);
            break;

        case swe_x_test:
            printf("swe_x_test dist=%d\n", ((int)(current[1]%gridlineSize - current[0]%gridlineSize)));

            if (((int)(current[1]%gridlineSize - current[0]%gridlineSize)) == 0)
            {
                next_state = swe_y_test;
            }
            else if (((int)(current[1]%gridlineSize - current[0]%gridlineSize)) > 0)
            {
                next_state = swe_xdec_test;
            }
            else
            {
                next_state = swe_xinc_test;
            }
            break;

        case swe_xdec_test:
            printf("swe_xdec_test oc-%d byp-%u fe-%d\n", grid[orientation_qnt*current[0] + orientation_right], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_right] || (bypass[current[0]] >= max_bypass && !firstEdge))
            {
                next_state = swe_y_test;
            }
            else
            {
                next_state = swe_xdec_set;
            }
            break;

        case swe_xdec_set:

            grid[orientation_qnt*current[0] + orientation_right] = true;

            if(!firstEdge)
            {
                bypass[current[0]]++;
            }

            next_current[0] = current[0] +1;
            next_modified = true;
            next_firstEdge = false;

            stackNode[stackIndex] = current[0];
            stackOutput[stackIndex] = orientation_right;
            next_stackIndex = stackIndex + 1;

            printf("swe_xdec_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = swe_x_test;
            break;

        case swe_xinc_test:
            printf("swe_xinc_test oc-%d byp-%u fe-%d\n", grid[current[0] + orientation_left], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_left] || (bypass[current[0]] > max_bypass && !firstEdge))
            {
                next_state = swe_y_test;
            }
            else
            {
                next_state = swe_xinc_set;
            }
            break;

        case swe_xinc_set:

            grid[orientation_qnt*current[0] + orientation_left] = true;

            if(!firstEdge)
            {
                bypass[current[0]]++;
            }

            next_current[0] = current[0] -1;
            next_modified = true;
            next_firstEdge = false;

            stackNode[stackIndex] = current[0];
            stackOutput[stackIndex] = orientation_left;
            next_stackIndex = stackIndex + 1;

            printf("swe_xinc_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = swe_x_test;
            break;

        case swe_y_test:
            printf("swe_y_test dist=%d\n", ((int)(current[1]/gridlineSize - current[0]/gridlineSize)));

            if (((int)(current[1]/gridlineSize - current[0]/gridlineSize)) == 0)
            {
                next_state = swe_xy_test;
            }
            else if (((int)(current[1]/gridlineSize - current[0]/gridlineSize)) > 0)
            {
                next_state = swe_ydec_test;
            }
            else
            {
                next_state = swe_yinc_test;
            }
            break;

        case swe_ydec_test:
            printf("swe_ydec_test oc-%d byp-%u fe-%d\n", grid[current[0] + orientation_left], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_bot] || (bypass[current[0]] >= max_bypass && !firstEdge))
            {
                next_state = swe_xy_test;
            }
            else
            {
                next_state = swe_ydec_set;
            }
            break;

        case swe_ydec_set:

            grid[orientation_qnt*current[0] + orientation_bot] = true;

            if(!firstEdge)
            {
                bypass[current[0]]++;
            }

            next_current[0] = current[0] +gridlineSize;
            next_modified = true;
            next_firstEdge = false;

            stackNode[stackIndex] = current[0];
            stackOutput[stackIndex] = orientation_bot;
            next_stackIndex = stackIndex + 1;

            printf("swe_ydec_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = swe_y_test;
            break;

        case swe_yinc_test:
            printf("swe_yinc_test oc-%d byp-%u fe-%d\n", grid[current[0] + orientation_top], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_top] || (bypass[current[0]] > max_bypass && !firstEdge))
            {
                next_state = swe_xy_test;
            }
            else
            {
                next_state = swe_yinc_set;
            }
            break;

        case swe_yinc_set:

            grid[orientation_qnt*current[0] + orientation_top] = true;

            if(!firstEdge)
            {
                bypass[current[0]]++;
            }

            next_current[0] = current[0] -gridlineSize;
            next_modified = true;
            next_firstEdge = false;

            stackNode[stackIndex] = current[0];
            stackOutput[stackIndex] = orientation_top;
            next_stackIndex = stackIndex + 1;

            printf("swe_yinc_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = swe_y_test;
            break;

        case swe_xy_test:
            printf("swe_xy_test %u==%u\n", current[0], current[1]);

            if (current[0] == current[1])
            {
                next_state = swe_nextedge;
            }
            else
            {
                next_state = swe_modified_test;
            }
            break;

        case swe_modified_test:
            printf("swe_modified_test mod=%d next_mod=%d\n", modified, next_modified);

            if (modified)
            {
                next_modified = false;
                next_state = swe_nextedge;
            }
            else
            {
                next_state = swe_blacklist;
            }
            break;

        case swe_blacklist:

            grid[orientation_qnt*stackNode[stackIndex] + stackOutput[stackIndex]] = false;

            if(stackIndex==0)
            {
                next_state = swe_nextedge;
            }
            else
            {
                next_stackIndex = stackIndex -1;
                bypass[stackNode[stackIndex]]--;

                next_state = swe_blacklist;
            }
            
            break;
        case swe_end:
            return;
            break;
        
        default:
            break;
        }

        // Always comb
        state = next_state;
        prefetch[0] = next_prefetch[0];
        prefetch[1] = next_prefetch[1];
        current[0] = next_current[0];
        current[1] = next_current[1];
        inputIndex = next_inputIndex;
        modified = next_modified;
        firstEdge = next_firstEdge;
        stackIndex = next_stackIndex;
    }
}