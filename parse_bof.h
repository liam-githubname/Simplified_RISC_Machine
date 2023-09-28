#ifndef _PARSE_BOF_H
#define _PARSE_BOF_H
#include "vm_mem.h"

// Formats the memory for the program and returns the mem_u variable containing it
extern union mem_u load_program(const char* filename);

#endif
