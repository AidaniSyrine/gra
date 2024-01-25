#pragma once 

#include <inttypes.h>
#include <stdio.h> 
#include "../core_logic/quadratic_interpolation.h"

#define ES  0      
#define AS  0    
#define EW  255    
#define AW  255 
#define A .2126
#define B .7152
#define C .0722
#define ARR_SIZE  4


void TEST_ALL_GAUSSIAN(void); 
void TEST_ALL_NEWTON(void);
void TEST_ALL_BLAGRANGE(void); 

void TEST_ALL_INTERPOLATION(void); 
