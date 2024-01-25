//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <immintrin.h>
#include <math.h>

//TODO write functions descriptions 
void img_to_gray_scale(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                           float a, float b, float c);

void img_to_gray_scale_SIMD(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                           float a, float b, float c);

