//
// Created by Adem Trabelsi on 21.12.23.
//

#include "grayscale.h"


void img_to_gray_scale(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                       float a, float b, float c) {
    //Next Update: SIMD opt.
    for (size_t i = 0; i < width * height; i++)
        gray_map[i] = (a * pix_map[3 * i] + b * pix_map[(3 * i) +1]
                + c * pix_map[3 * i + 2]) / (a+b+c);
}

void img_to_gray_scale_SIMD(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                               float a, float b, float c) {
}

