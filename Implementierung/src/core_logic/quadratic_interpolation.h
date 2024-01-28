//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <math.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>

#define ROUNDING 0.000001

//TODO Write functions descriptions 

void row_reduced_echelon_form(int num_rows, int num_cols, float matrix[num_rows][num_cols]); 

void quadratic_interpolation_gaussian(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw);
//TODO
void quadratic_interpolation_gaussian_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw, int out_of_bound);

void quadratic_interpolation_lagrange(uint8_t* gray_map, size_t width, size_t height,
                                       float es, float as, float em,
                                       float am, float ew, float aw);

void quadratic_interpolation_blagrange(uint8_t* gray_map, size_t width, size_t height,
                                       float es, float as, float em,
                                       float am, float ew, float aw);

void quadratic_interpolation_newton(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw);

void quadratic_interpolation_newton_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw);
//TODO
void quadratic_interpolation_newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                        float es, float as, float em,
                                        float am, float ew, float aw , int out_of_bound);




