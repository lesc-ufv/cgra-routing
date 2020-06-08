#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

#define orientation_qnt 4
#define empty_pe 255

#define orientation_top 0
#define orientation_bot 1
#define orientation_left 2
#define orientation_right 3

unsigned int floorSqrt(unsigned int x);

typedef struct
{
    bool output [orientation_qnt];
    unsigned int bypass;
} PE;

void PEInitialize(PE * pe);
void PEPrint(PE * pe);

typedef struct
{
    PE * grid;
    unsigned int maxBypass;
    unsigned int gridSize;
} CGRA;

void CGRAInitialize(CGRA * cgra, unsigned int maxBypass, const char * gridFilename);
void CGRAFree(CGRA * cgra);
bool CGRAEdgeIsTrivial(CGRA * cgra, unsigned int source, unsigned int destination);
void CGRAEdgeConnectTrivial(CGRA * cgra, unsigned int source, unsigned int destination);
void CGRAPrint(CGRA * cgra);
void CGRAGridCopy(CGRA * copy, bool * paste);
void CGRABypassCopy(CGRA * copy, unsigned int * paste);

typedef struct
{
    unsigned int * vector;
    unsigned int gridSize;
} MaskVector;

void MaskVectorInitialize(MaskVector * vector, CGRA * cgra, FILE * gridFile, FILE * output);
unsigned int MaskVectorConvert(MaskVector * vector, unsigned int position);
void MaskVectorPrint(MaskVector * vector);
void MaskVectorFree(MaskVector * vector);

typedef struct
{
    unsigned int * vector;
    unsigned int inputQnt;
} InputEdgesVector;

void InputEdgesVectorInitialize(InputEdgesVector * input, MaskVector * mask, CGRA * cgra, const char * edgeFilename, FILE * outputFile);
void InputEdgesVectorPrint(InputEdgesVector * vector);
void InputEdgesVectorFree(InputEdgesVector * vector);
void InputEdgesVectorCopy(InputEdgesVector * copy, unsigned int * paste);

#endif