
#pragma once

#include "../core_logic/grayscale.h"
#include "../core_logic/linear_interpolation.h"
#include "../core_logic/quadratic_interpolation.h"
#include "../core_logic/adjustment.h"
#include "../io_utilities/img_io.h"

#include <assert.h>
#include <math.h>

int testImageSimilarity(uint8_t* image1, uint8_t* image2,size_t size);
void test_grayscale_SIMD(Image_params* pix_map);
void test_linear_SIMD(Image_params* pix_map);
void test_quadraticLS_SIMD(Image_params* pix_map);
void test_newton_SIMD(Image_params* pix_map);
void test_quadratic(Image_params* pix_map) ;

