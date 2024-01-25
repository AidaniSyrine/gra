//
// Created by Adem Trabelsi on 12.01.24.
//
#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>

//TODO Write functions descriptions 

void linear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw);

void linear_interpolation_LUT(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw);

void linear_interpolation_SIMD(uint8_t* gray_map, size_t width, size_t height,
                               uint8_t es, uint8_t as, uint8_t em,
                               uint8_t am, uint8_t ew, uint8_t aw);

void bilinear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                            uint8_t es, uint8_t as, uint8_t em,
                            uint8_t am, uint8_t ew, uint8_t aw);

void bilinear_interpolation_LUT(uint8_t* gray_map, size_t width, size_t height,
                                uint8_t es, uint8_t as, uint8_t em,
                                uint8_t am, uint8_t ew, uint8_t aw);
            