#include "common.h"

unsigned int floorSqrt(unsigned int x)
{
    // Base cases
    if (x == 0 || x == 1)
        return x;

    // Staring from 1, try all numbers until
    // i*i is greater than or equal to x.
    unsigned int i = 1, result = 1;
    while (result <= x)
    {
        i++;
        result = i * i;
    }
    return i - 1;
}

void PEInitialize(PE *pe)
{
    for (size_t i = 0; i < orientation_qnt; i++)
    {
        pe->output[i] = false;
    }

    pe->bypass = 0;
}

void PEPrint(PE *pe)
{
    printf("%d%d%d%d - %u\n", pe->output[orientation_top], pe->output[orientation_bot], pe->output[orientation_left], pe->output[orientation_right], pe->bypass);
}

void CGRAInitialize(CGRA *cgra, unsigned int maxBypass, const char *gridFilename)
{
    unsigned int gridSize = 0;
    FILE *gridFile = fopen(gridFilename, "r");
    char swap;

    fscanf(gridFile, "\n");
    do
    {
        fscanf(gridFile, "%c", &swap);
        if (swap == ' ')
        {
            gridSize++;
        }

    } while (swap != '\n');

    cgra->grid = malloc(gridSize * sizeof(PE));
    cgra->gridSize = gridSize;
    cgra->maxBypass = maxBypass;

    for (size_t i = 0; i < gridSize; i++)
    {
        PEInitialize(&(cgra->grid[i]));
    }
}

void CGRAFree(CGRA *cgra)
{
    free(cgra->grid);
}

bool CGRAEdgeIsTrivial(CGRA *cgra, unsigned int source, unsigned int destination)
{
    unsigned int gridline = floorSqrt(cgra->gridSize);
    unsigned int distance = abs((int)(destination - source));

    if (distance == 1 || distance == gridline)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CGRAEdgeConnectTrivial(CGRA *cgra, unsigned int source, unsigned int destination)
{
    int gridline = (int)floorSqrt(cgra->gridSize);
    int distance = (int)(destination - source);

    if (distance == 1)
    {
        cgra->grid[source].output[orientation_right] = true;
    }
    else if (distance == -1)
    {
        cgra->grid[source].output[orientation_left] = true;
    }
    else if (distance == gridline)
    {
        cgra->grid[source].output[orientation_bot] = true;
    }
    else if (distance == (-1) * gridline)
    {
        cgra->grid[source].output[orientation_top] = true;
    }
}

void CGRAPrint(CGRA *cgra)
{
    printf("[N] TBLR - B\n");
    for (size_t i = 0; i < cgra->gridSize; i++)
    {
        printf("[%lu] ", i);
        PEPrint(&(cgra->grid[i]));
    }
}

void CGRAGridCopy(CGRA *copy, bool *paste)
{
    unsigned int count = 0;

    for (size_t i = 0; i < copy->gridSize; i++)
    {
        paste[count] = copy->grid[i].output[0];
        paste[count + 1] = copy->grid[i].output[1];
        paste[count + 2] = copy->grid[i].output[2];
        paste[count + 3] = copy->grid[i].output[3];

        count = count + 4;
    }
}

void CGRABypassCopy(CGRA *copy, unsigned int *paste)
{
    for (size_t i = 0; i < copy->gridSize; i++)
    {
        paste[i] = copy->grid[i].bypass;
    }
}

void MaskVectorInitialize(MaskVector *vector, CGRA *cgra, FILE *gridFile, FILE *output)
{
    unsigned int count = 0;
    unsigned int swap;

    unsigned int debug_empty = 0;

    vector->gridSize = cgra->gridSize;
    vector->vector = malloc(vector->gridSize * sizeof(unsigned int));

    fscanf(gridFile, "\n");
    for (size_t i = 0; i < vector->gridSize; i++)
    {
        fscanf(gridFile, "%u ", &swap);

        if (swap != empty_pe)
        {
            vector->vector[swap] = count;
            (cgra->grid[count].bypass)++;
        }
        else
        {
            debug_empty++;
        }

        count++;
    }

    fprintf(output, "%u,%u,", cgra->gridSize, debug_empty);
}

unsigned int MaskVectorConvert(MaskVector *vector, unsigned int position)
{
    return vector->vector[position];
}

void MaskVectorPrint(MaskVector *vector)
{
    for (size_t i = 0; i < vector->gridSize; i++)
    {
        printf("%u ", vector->vector[i]);
    }
    printf("\n");
}

void MaskVectorFree(MaskVector *vector)
{
    free(vector->vector);
}

void InputEdgesVectorInitialize(InputEdgesVector *input, MaskVector *mask, CGRA *cgra, const char *edgeFilename, FILE *outputFile)
{
    unsigned int debug_trivials = 0;

    unsigned int count = 0;
    unsigned swapSource, swapDestination;
    input->inputQnt = 0;
    FILE *edgeFile = fopen(edgeFilename, "r");

    fscanf(edgeFile, "%u %u\n\n", &swapSource, &swapDestination); // Ignoring edge count and grid size

    while (fscanf(edgeFile, "%u %u\n", &swapSource, &swapDestination) != EOF)
    {
        swapSource = MaskVectorConvert(mask, swapSource);
        swapDestination = MaskVectorConvert(mask, swapDestination);

        if (CGRAEdgeIsTrivial(cgra, swapSource, swapDestination))
        {
            CGRAEdgeConnectTrivial(cgra, swapSource, swapDestination);
            debug_trivials++;
        }
        else
        {
            input->inputQnt++;
        }
    }

    fprintf(outputFile, "%u, %u, ", debug_trivials, input->inputQnt);

    edgeFile = fopen(edgeFilename, "r");
    input->vector = malloc(2 * (input->inputQnt + 1) * sizeof(unsigned int));
    fscanf(edgeFile, "%u %u\n\n", &swapSource, &swapDestination); // Ignoring edge count and grid size

    while (fscanf(edgeFile, "%u %u\n", &swapSource, &swapDestination) != EOF)
    {
        swapSource = MaskVectorConvert(mask, swapSource);
        swapDestination = MaskVectorConvert(mask, swapDestination);

        if (!(CGRAEdgeIsTrivial(cgra, swapSource, swapDestination)))
        {
            input->vector[count] = swapSource;
            input->vector[count + 1] = swapDestination;
            count = count + 2;
        }
    }

    input->vector[count] = 0;
    input->vector[count + 1] = 0;
    input->inputQnt++;

    fclose(edgeFile);
}

void InputEdgesVectorPrint(InputEdgesVector *vector)
{
    for (size_t i = 0; i < 2 * (vector->inputQnt); i = i + 2)
    {
        printf("%.2u - %.2u -- Distance = %d\n", vector->vector[i], vector->vector[i + 1], (int)vector->vector[i + 1] - vector->vector[i]);
    }
}

void InputEdgesVectorFree(InputEdgesVector *vector)
{
    free(vector->vector);
}

void InputEdgesVectorCopy(InputEdgesVector *copy, unsigned int *paste)
{
    for (size_t i = 0; i < 2 * (copy->inputQnt + 1); i = i + 2)
    {
        paste[i] = copy->vector[i];
        paste[i + 1] = copy->vector[i + 1];
    }
}

void CreateLegupInput(CGRA *cgra, InputEdgesVector *input)
{
    printf("unsigned int input[%u] = {", (*input).inputQnt);
    for (size_t i = 0; i < (*input).inputQnt; i++)
    {
        if (i != ((*input).inputQnt - 1))
        {
            printf("%u, ", (*input).vector[i]);
        }
        else
        {
            printf("%u}\n", (*input).vector[i]);
        }
    }

    printf("bool grid[%u] = {", 4 * (*cgra).gridSize);
    for (size_t i = 0; i < (*cgra).gridSize; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (i != ((*cgra).gridSize - 1) || j != 3)
            {
                printf("%u, ", (*cgra).grid[i].output[j]);
            }
            else
            {
                printf("%u}\n", (*cgra).grid[i].output[j]);
            }
        }
    }

    printf("unsigned int bypass[%u] = {", (*cgra).gridSize);
    for (size_t i = 0; i < (*cgra).gridSize; i++)
    {
        if (i != ((*cgra).gridSize - 1))
        {
            printf("%u, ", (*cgra).grid[i].bypass);
        }
        else
        {
            printf("%u}\n", (*cgra).grid[i].bypass);
        }
    }

    printf("unsigned int stackNode[%u];\n", 2 * (floorSqrt((*cgra).gridSize) - 1));
    printf("unsigned int stackOutput[%u];\n", 2 * (floorSqrt((*cgra).gridSize) - 1));
}