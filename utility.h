#ifndef __UTILITY_H
#define __UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <CL/cl.h>


#define EPSILON (1e-2f)

void print_platform_info(cl_platform_id*);
void print_device_info(cl_device_id*);
void allocate_generate(void** X, void** Y, void** Z, float LO, float HI, size_t vectorSize);
bool verification (void * X, void * Y, void * Z, void * CalcZ, size_t vectorSize);

unsigned char* get_binary(const char * name, size_t* length);

#endif
