//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <math.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>



//TODO Write functions descriptions 


/// @brief Assume that the LS is solvable for all 
/// entries
/// @param x 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
void gaussian_elimination(float* x,  float es, float as,
                        float em, float am, float ew, float aw );

void quadratic_interpolation_Gaussian(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw, int out_of_bound);

void quadratic_interpolation_Gaussian_LUT(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw, int out_of_bound);

void quadratic_interpolation_Gaussian_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw, int out_of_bound);

void quadratic_interpolation_BLagrange(uint8_t* gray_map, size_t width, size_t height,
                                      uint8_t es, uint8_t as, uint8_t em,
                                      uint8_t am, uint8_t ew, uint8_t aw);

void quadratic_interpolation_BLagrange_LUT(uint8_t* gray_map, size_t width, size_t height,
                                      uint8_t es, uint8_t as, uint8_t em,
                                      uint8_t am, uint8_t ew, uint8_t aw);

void quadratic_interpolation_Newton(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw, int out_of_bound);

void quadratic_interpolation_Newton_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw, int out_of_bound);

void quadratic_interpolation_Newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                        float es, float as, float em,
                                        float am, float ew, float aw,int out_of_bound);

int is_out_of_bound (float es, float as, float em, float am, float ew, float aw);



