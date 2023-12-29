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


// TODO ??
void img_to_gray_scale_LUT(void) {
    return;
}


void img_to_gray_scale_SIMD(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                               float a, float b, float c) {
    const __m128 a_coeff = _mm_set1_ps(a);
    const __m128 b_coeff = _mm_set1_ps(b);
    const __m128 c_coeff = _mm_set1_ps(c);
    const __m128 div = _mm_set1_ps(a + b + c);

    size_t i, s;
    size_t num_pix = width * height * 3 * sizeof(uint8_t);

    for (i = 0 ,s = 0; i < num_pix - (num_pix % 12); i += 12, s += 4) {

        // Load R from 4 pixels
        __m128 a_col = _mm_set_ps(pix_map[i + 9], pix_map[i + 6], pix_map[i + 3], pix_map[i]);
        // Load G from 4 pixels
        __m128 b_col = _mm_set_ps(pix_map[i + 10], pix_map[i + 7], pix_map[i + 4], pix_map[i + 1]);
        // Load B from 4 pixels
        __m128 c_col = _mm_set_ps(pix_map[i + 11], pix_map[i + 8], pix_map[i + 5], pix_map[i + 2]);

        // Multiply each color with the coeff
        a_col = _mm_mul_ps(a_col, a_coeff);
        b_col = _mm_mul_ps(b_col, b_coeff);
        c_col = _mm_mul_ps(c_col, c_coeff);

        // Divide the sum by (a+b+c)
        __m128 result  = _mm_div_ps(_mm_add_ps(a_col, _mm_add_ps(b_col, c_col)), div);

        // Store the 4 calculated values in gray_map
        __m128i final_result = _mm_cvtps_epi32(result);

        final_result = _mm_packus_epi16(_mm_packus_epi32(final_result, _mm_setzero_si128()), _mm_setzero_si128());
        _mm_storeu_si32(gray_map+s,final_result);
    }

    // Calculate the gray value iteratively
    for ( ;s < width * height; s++)
        gray_map[s]=(a * pix_map[3 * s] + b * pix_map[3 * s + 1]+ c * pix_map[3 * s + 2]) / (a + b + c);

    return;
}



