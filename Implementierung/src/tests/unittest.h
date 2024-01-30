#pragma once 

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../core_logic/quadratic_interpolation.h"
#include "../io_utilities/img_io.h"


#define assert__(x) for ( ; !(x) ; assert(x) )

void __ASSERT__EQUAL__ARRAY(uint8_t*  expected, uint8_t* actual, size_t len, char* message);
void __ASSERT__SIMILAR__ARRAY(uint8_t* expected, uint8_t* actual, size_t len, char* message);
void __ASSERT__EQUAL(int expected, int actual, char* message); 
void __ASSERT__EQUAL__IMGPARAMS(Image_params* expected, Image_params* actual, char* message); 

void start_testing(char* message); 
void end_testing();