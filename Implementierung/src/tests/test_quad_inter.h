#pragma once 

#include <inttypes.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "unit.h"
#include "../core_logic/quadratic_interpolation.h"
#include "../core_logic/linear_interpolation.h"
#include "../core_logic/grayscale.h"
#include "../io_utilities/arg_parser.h"

#define ARR_SIZE 12288 // 3 Unix pages //! Must be divisible by 3



void TEST_QUAD_INTERPOLATION(void); 
