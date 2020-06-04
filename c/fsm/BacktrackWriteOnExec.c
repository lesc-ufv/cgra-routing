#include "BacktrackWriteOnExec.h"

#define bwe_init 0
#define bwe_nextedge 1
#define bwe_end 2

#define bwe_x_test 3
#define bwe_xinc_test 4
#define bwe_xinc_set 5
#define bwe_xdec_test 6
#define bwe_xdec_set 7

#define bwe_y_test 8
#define bwe_yinc_test 9
#define bwe_yinc_set 10
#define bwe_ydec_test 11
#define bwe_ydec_set 12

#define bwe_xy_test 13
#define bwe_modified_test 14

#define bwe_backtrack 15

void FSM_BacktrackWriteOnExec(CGRA * out_grid, InputEdgesVector * out_input, FILE * out_output)
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

    InputEdgesVectorCopy(out_input, input);
    CGRAGridCopy(out_grid, grid);
    CGRABypassCopy(out_grid, bypass);

    // Internals
    unsigned int state;
    unsigned int current[2];
    unsigned int inputIndex, stackIndex;
    int backtrackIndex;
    bool modified, firstEdge, backtracked;

    // Next internals
    unsigned int next_state;
    unsigned int next_current[2];
    unsigned int next_inputIndex, next_stackIndex;
    int next_backtrackIndex;
    bool next_modified, next_firstEdge, next_backtracked;

    // Simulating reset pressed
    state = bwe_init;

    // DEBUG
    unsigned int debug_clock = 0;
    unsigned int debug_bl = 0;
    unsigned int debug_routed = 0;
    unsigned int debug_usedOutputs = 0;

    while (true)
    {
        switch (state)
        {

        case bwe_init:
            DEBUG_PRINT("bwe_init\n");

            next_inputIndex = 0;

            next_state = bwe_nextedge;
            break;

        case bwe_nextedge:


            if(input[inputIndex + 0]==0 && input[inputIndex + 1]==0)
            {
                next_state = bwe_end;
            }
            else
            {
                next_current[0] = input[inputIndex + 0];
                next_current[1] = input[inputIndex + 1];

                next_inputIndex = inputIndex + 2;
                next_stackIndex = 0;
                next_firstEdge = true;
                next_modified = false;

                next_backtracked = false;

                next_state = bwe_x_test;
    
                
            }
            DEBUG_PRINT("bwe_nextedge C[%u-%u]\n", next_current[0], next_current[1]);
            break;

        case bwe_x_test:
            DEBUG_PRINT("bwe_x_test dist=%d\n", ((int)(current[1]%gridlineSize - current[0]%gridlineSize)));

            if (((int)(current[1]%gridlineSize - current[0]%gridlineSize)) == 0)
            {
                next_state = bwe_y_test;
            }
            else if (((int)(current[1]%gridlineSize - current[0]%gridlineSize)) > 0)
            {
                next_state = bwe_xdec_test;
            }
            else
            {
                next_state = bwe_xinc_test;
            }
            break;

        case bwe_xdec_test:
            DEBUG_PRINT("bwe_xdec_test oc-%d byp-%u fe-%d\n", grid[orientation_qnt*current[0] + orientation_right], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_right] || (bypass[current[0]] >= max_bypass && !firstEdge))
            {
                next_state = bwe_y_test;
            }
            else
            {
                next_state = bwe_xdec_set;
            }
            break;

        case bwe_xdec_set:

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

            DEBUG_PRINT("bwe_xdec_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = bwe_x_test;
            break;

        case bwe_xinc_test:
            DEBUG_PRINT("bwe_xinc_test oc-%d byp-%u fe-%d\n", grid[current[0] + orientation_left], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_left] || (bypass[current[0]] > max_bypass && !firstEdge))
            {
                next_state = bwe_y_test;
            }
            else
            {
                next_state = bwe_xinc_set;
            }
            break;

        case bwe_xinc_set:

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

            DEBUG_PRINT("bwe_xinc_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = bwe_x_test;
            break;

        case bwe_y_test:
            DEBUG_PRINT("bwe_y_test dist=%d\n", ((int)(current[1]/gridlineSize - current[0]/gridlineSize)));

            if (((int)(current[1]/gridlineSize - current[0]/gridlineSize)) == 0)
            {
                next_state = bwe_xy_test;
            }
            else if (((int)(current[1]/gridlineSize - current[0]/gridlineSize)) > 0)
            {
                next_state = bwe_ydec_test;
            }
            else
            {
                next_state = bwe_yinc_test;
            }
            break;

        case bwe_ydec_test:
            DEBUG_PRINT("bwe_ydec_test oc-%d byp-%u fe-%d\n", grid[current[0] + orientation_left], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_bot] || (bypass[current[0]] >= max_bypass && !firstEdge))
            {
                next_state = bwe_xy_test;
            }
            else
            {
                next_state = bwe_ydec_set;
            }
            break;

        case bwe_ydec_set:

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

            DEBUG_PRINT("bwe_ydec_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = bwe_y_test;
            break;

        case bwe_yinc_test:
            DEBUG_PRINT("bwe_yinc_test oc-%d byp-%u fe-%d\n", grid[current[0] + orientation_top], bypass[current[0]], firstEdge);

            if(grid[orientation_qnt*current[0] + orientation_top] || (bypass[current[0]] > max_bypass && !firstEdge))
            {
                next_state = bwe_xy_test;
            }
            else
            {
                next_state = bwe_yinc_set;
            }
            break;

        case bwe_yinc_set:

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

            DEBUG_PRINT("bwe_yinc_set C[%u-%u]\n", next_current[0], next_current[1]);
            next_state = bwe_y_test;
            break;

        case bwe_xy_test:
            DEBUG_PRINT("bwe_xy_test %u==%u\n", current[0], current[1]);

            if (current[0] == current[1])
            {
                debug_routed++;
                next_state = bwe_nextedge;
            }
            else
            {
                next_state = bwe_modified_test;
            }
            break;

        case bwe_modified_test:
            DEBUG_PRINT("bwe_modified_test modified=%d\n", modified);

            if (modified)
            {
                next_modified = false;
                next_state = bwe_x_test;
            }
            else
            {
                next_state = bwe_backtrack;
            }
            break;

        case bwe_backtrack:
            DEBUG_PRINT("bwe_backtrack IE=%u\n", stackIndex);

            grid[orientation_qnt*stackNode[stackIndex] + stackOutput[stackIndex]] = false;

            if(!backtracked)
            {
                DEBUG_PRINT("FIRST BACKTRACKING - %d\n", backtracked);
                next_backtrackIndex = stackIndex - 1;
                next_backtracked = true;
                next_state = bwe_backtrack;
            }
            else
            {
                DEBUG_PRINT("BACKTRACK NUMBER - %d == %d\n", backtrackIndex, backtracked);
                if(backtrackIndex==-1)
                {
                    debug_bl++;
                    next_state = bwe_nextedge;
                }
                else if(stackOutput[backtrackIndex]==orientation_bot || stackOutput[backtrackIndex]==orientation_top)
                {
                    next_state = bwe_x_test;
                }
                else
                {
                    next_state = bwe_y_test;
                }

                grid[orientation_qnt*stackNode[backtrackIndex]+stackOutput[backtrackIndex]] = false;
                bypass[stackNode[backtrackIndex]]--;
                next_backtrackIndex = backtrackIndex - 1;
            }
            
            
            break;
        case bwe_end:
            DEBUG_PRINT("bwe_end\n");

            for (size_t i = 0; i < 4*out_grid->gridSize; i++)
            {
                if(grid[i])
                {
                    debug_usedOutputs++;
                }
            }

            DEBUG_PRINT("[DEBUG] %u, %u, %u, %f\n", debug_clock, debug_bl, debug_routed, (double)debug_usedOutputs/(out_grid->gridSize*4));
            fprintf(out_output, "%u, %u, %u, %f", debug_clock, debug_bl, debug_routed, (double)debug_usedOutputs/(out_grid->gridSize*4));
            
            return;
            break;
        
        default:
            break;
        }

        // Always comb
        state = next_state;
        current[0] = next_current[0];
        current[1] = next_current[1];
        inputIndex = next_inputIndex;
        modified = next_modified;
        firstEdge = next_firstEdge;
        stackIndex = next_stackIndex;
        backtracked = next_backtracked;
        backtrackIndex = next_backtrackIndex;

        debug_clock++;
    }
}