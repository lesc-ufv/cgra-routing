#ifndef SIMPLEWRITEONEXEC_H
#define SIMPLEWRITEONEXEC_H

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

#include "common.h"

void FSM_SimpleWriteOnExec(CGRA * grid, InputEdgesVector * input, FILE * output);

#endif