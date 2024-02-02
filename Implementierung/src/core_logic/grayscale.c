//
// Created by Adem Trabelsi on 21.12.23.
//

#include "grayscale.h"
 #include <stdio.h>

void img_to_gray_scale(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                       float a, float b, float c) {
    for (size_t i = 0; i < width * height; i++)
        gray_map[i] = (a * pix_map[3 * i] + b * pix_map[3 * i +1]
                + c * pix_map[3 * i + 2]) / (a + b + c);
}

//Credits to https://stackoverflow.com/a/57844027

void img_to_gray_scale_SIMD(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height, float a, float b, float c) {
        const __m128i mask = _mm_set_epi8(9, 6, 3, 0, 11, 8, 5, 2, 10, 7, 4, 1, 9, 6, 3, 0);
        
        const __m128i as = _mm_set1_epi16((short)((a / (a + b + c)) * 32768.0 + 0.5)); 
        const __m128i bs = _mm_set1_epi16((short)((b / (a + b + c)) * 32768.0 + 0.5)); 
        const __m128i cs = _mm_set1_epi16((short)((c / (a + b + c)) * 32768.0 + 0.5)); 
        
        size_t i;
        size_t s;
        size_t size = height * width;

        for (i = 0, s = 0; s < size - (size % 8); i+=24, s+=8) {
        __m128i r5r0 = _mm_loadu_si128((const __m128i *) &pix_map[i]);
        __m128i b7g5 = _mm_loadu_si128((const __m128i *) &pix_map[i + 16]);

        //RGB to RRRRGGGGBBBBB
        __m128i rr3rr0bb3rr0 = _mm_shuffle_epi8(r5r0, mask);
        __m128i rr7rr4bb7rr4 = _mm_shuffle_epi8(_mm_alignr_epi8 (b7g5, r5r0, 12), mask);

        //8R in lower part
        __m128i x_r7r0 = _mm_alignr_epi8(rr7rr4bb7rr4, rr3rr0bb3rr0, 12);

        //8G in lower part
        __m128i gg3rr0_x = _mm_slli_si128(rr3rr0bb3rr0, 8);
        __m128i x_rr7gg4 = _mm_srli_si128(rr7rr4bb7rr4, 4);
        __m128i x_g8g0 = _mm_alignr_epi8(x_rr7gg4, gg3rr0_x, 12);

        //8B in lower part
        __m128i bb3rr0_x = _mm_slli_si128(rr3rr0bb3rr0, 4);
        __m128i x_rr7bb4 = _mm_srli_si128(rr7rr4bb7rr4, 8);
        __m128i x_b8b0 = _mm_alignr_epi8(x_rr7bb4, bb3rr0_x, 12);

        //uint8_t to uint16_t
        __m128i _8rs= _mm_cvtepu8_epi16(x_r7r0);
        __m128i _8gs = _mm_cvtepu8_epi16(x_g8g0);
        __m128i _8bs = _mm_cvtepu8_epi16(x_b8b0);

        // Shift right by 64 aka. x64 to improve accuracy 
        _8rs = _mm_slli_epi16(_8rs, 6);
        _8gs = _mm_slli_epi16(_8gs, 6);
        _8bs = _mm_slli_epi16(_8bs, 6);

        // Luminance
        __m128i result_epi16 = _mm_add_epi16(_mm_add_epi16(_mm_mulhrs_epi16(_8rs, as), _mm_mulhrs_epi16(_8gs, bs)), _mm_mulhrs_epi16(_8bs, cs));

        // Shift left by 64 aka. /64 -> actual value
        result_epi16 = _mm_srli_epi16(result_epi16, 6);

        // uint16_t to uint8_t 
        __m128i result_epi8 =  _mm_packus_epi16(result_epi16, result_epi16);

        // store lower 8 uint8_t
        _mm_storel_epi64((__m128i *)&gray_map[s], result_epi8);

    }

    // iterate over the rest    
    for ( ; s < width * height; s++) { 
        gray_map[s] =
            a * pix_map[3 * s] + b * pix_map[3 * (s + 1)] + c * pix_map[3 * (s + 2)]; 
    }
}
