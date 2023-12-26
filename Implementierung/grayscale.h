//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <stdlib.h>

#define A .299
#define B .587
#define C .114

/**
 *
 * @param pix_map
 * @param width
 * @param height
 * @return
 */
void img_to_gray_scale(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                           float a, float b, float c);

void img_to_gray_scale_SIMD(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                           float a, float b, float c);