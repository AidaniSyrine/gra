//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <immintrin.h>

/**
 *
 * @param pix_map
 * @param width
 * @param height
 * @return
 */
uint8_t* img_to_grey_scale(uint8_t* pix_map, int width, int height,
                           float a, float b, float c);

