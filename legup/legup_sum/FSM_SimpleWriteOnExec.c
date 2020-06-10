#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define swe_init 0
#define swe_nextedge 1
#define swe_end 2

#define swe_x_test 3
#define swe_xinc_test 4
#define swe_xinc_set 5
#define swe_xdec_test 6
#define swe_xdec_set 7

#define swe_y_test 8
#define swe_yinc_test 9
#define swe_yinc_set 10
#define swe_ydec_test 11
#define swe_ydec_set 12

#define swe_xy_test 13
#define swe_modified_test 14

#define swe_blacklist 15

#define orientation_qnt 4
#define empty_pe 255

#define orientation_top 0
#define orientation_bot 1
#define orientation_left 2
#define orientation_right 3

void main()
{
    // Constants
    unsigned int max_bypass = 2;
    unsigned int gridlineSize = 4;

    // Memories
unsigned int input[12] = {6, 6, 6, 4, 3, 1, 3, 5, 1, 1, 0, 0};
bool grid[36] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
unsigned int bypass[9] = {0, 1, 1, 1, 1, 1, 1, 1, 0};
unsigned int stackNode[4];
unsigned int stackOutput[4];

    // Internals
    unsigned int state;
    unsigned int current[2];
    unsigned int inputIndex, stackIndex;
    bool modified, firstEdge;

    // Next internals
    unsigned int next_state;
    unsigned int next_current[2];
    unsigned int next_inputIndex, next_stackIndex;
    bool next_modified, next_firstEdge;

    // Simulating reset pressed
    state = swe_init;

    while (true)
    {
        switch (state)
        {

        case swe_init:
            next_inputIndex = 0;

            next_state = swe_nextedge;
            break;

        case swe_nextedge:


            if(input[inputIndex + 0]==0 && input[inputIndex + 1]==0)
            {
                next_state = swe_end;
            }
            else
            {
                next_current[0] = input[inputIndex + 0];
                next_current[1] = input[inputIndex + 1];

                next_inputIndex = inputIndex + 2;
                next_stackIndex = 0;
                next_firstEdge = true;
                next_modified = false;

                next_state = swe_x_test;
            }
            break;

        case swe_x_test:

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

            next_state = swe_x_test;
            break;

        case swe_xinc_test:

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

            next_state = swe_x_test;
            break;

        case swe_y_test:

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

            next_state = swe_y_test;
            break;

        case swe_yinc_test:

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

            next_state = swe_y_test;
            break;

        case swe_xy_test:

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

            if (modified)
            {
                next_modified = false;
                next_state = swe_x_test;
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
            next_state = swe_end;
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
    }
}

