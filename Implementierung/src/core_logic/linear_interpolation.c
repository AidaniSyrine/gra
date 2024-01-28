//
// Created by Adem Trabelsi on 12.01.24.
//

#include "linear_interpolation.h"




void linear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw)
{
   for(size_t i = 0; i < width * height; i++) {
       if (gray_map[i] <= es) gray_map[i] = as;
       else if (gray_map[i] >= ew) gray_map[i] = aw;
       else if (gray_map[i] == em) gray_map[i] = am;
       else if (gray_map[i] <= em)
           gray_map[i] = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
       else
           gray_map[i] = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
    }
}

void linear_interpolation_SIMD(uint8_t* gray_map, size_t width, size_t height,
                               uint8_t es, uint8_t as, uint8_t em,
                               uint8_t am, uint8_t ew, uint8_t aw)
{
    __m128i simd_es = _mm_set1_epi16(es);
    __m128i simd_as = _mm_set1_epi8(as);
    __m128i simd_em = _mm_set1_epi16(em);
    __m128i simd_am = _mm_set1_epi8(am);
    __m128i simd_ew = _mm_set1_epi16(ew);

    __m128i simd_factor1 = _mm_set1_epi8((am - as) / (em - es));
    __m128i simd_factor2 = _mm_set1_epi8((uint8_t)((aw - am) / (ew - em)));

    size_t size = width * height;
    size_t i;
    for (i = 0; i < size - (size % 16); i += 16) {
        // Load 16 bytes 
        __m128i simd_gray = _mm_loadu_si128((__m128i*)(gray_map+i));

        // Change bytes <es to 0
        __m128i mask_es = _mm_cmpgt_epi16(_mm_unpacklo_epi8(simd_gray, _mm_setzero_si128()), simd_es); 
        mask_es = _mm_packs_epi16 (mask_es,_mm_cmpgt_epi16(_mm_unpackhi_epi8(simd_gray,
                                                                             _mm_setzero_si128()), simd_es));
        mask_es = _mm_andnot_si128(mask_es, _mm_set1_epi8((uint8_t) 0xff));
        __m128i result = _mm_blendv_epi8(simd_gray, _mm_set1_epi8(0),  mask_es);
        
        // Change bytes >ew to 0
        __m128i mask_ew = _mm_cmplt_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()), simd_ew);
        mask_ew = _mm_packs_epi16 (mask_ew,_mm_cmplt_epi16(_mm_unpackhi_epi8(result,
                                                                             _mm_setzero_si128()), simd_ew));
        mask_ew = _mm_andnot_si128(mask_ew, _mm_set1_epi8((uint8_t) 0xff));
        result = _mm_blendv_epi8(result, _mm_set1_epi8(0),  mask_ew);

        // Initialise new vector factor with simd_factor1 for bytes <em and with simd_factor2 for bytes >em
        __m128i mask_ltem = _mm_cmplt_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()), simd_em);
        mask_ltem = _mm_packs_epi16 (mask_ltem,_mm_cmplt_epi16(_mm_unpackhi_epi8(result,
                                                                                 _mm_setzero_si128()), simd_em));
        __m128i factors = _mm_and_si128(mask_ltem, simd_factor1 );
        __m128i mask_getem =_mm_andnot_si128(mask_ltem, _mm_set1_epi8((uint8_t) 0xff));
        factors = _mm_or_si128(factors, _mm_and_si128(mask_getem, simd_factor2 ));

        // Initialise new vector adds_constants with simd_as for bytes <em and with simd_am for bytes >em 
        __m128i adds_constants = _mm_and_si128(mask_ltem, simd_as ) ;
        adds_constants = _mm_or_si128(adds_constants, _mm_and_si128(mask_getem, simd_am ));

        // Initialise new vector sub_constants with es*16 for bytes <em and em*16 for bytes >em
        __m128i sub_constants = _mm_and_si128(mask_ltem, _mm_set1_epi8(es)) ;
        sub_constants = _mm_or_si128(sub_constants, _mm_and_si128(mask_getem, _mm_set1_epi8(em)));

        // Subtraction aka. gray_map[_] - es | gray_map[_] - em  
        result = _mm_sub_epi8 (result, sub_constants);

        // Multiplikation aka. factor * (gray_map[_] - es | gray_map[_] - em)  
        __m128i final_result1 = _mm_maddubs_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()),
                                                  _mm_unpacklo_epi8(factors, _mm_setzero_si128()));
        __m128i final_result2 = _mm_maddubs_epi16(_mm_unpackhi_epi8(result, _mm_setzero_si128()),
                                                  _mm_unpackhi_epi8(factors, _mm_setzero_si128()));
        final_result1 = _mm_packus_epi16 (final_result1, final_result2);

        // Addition aka. +am | +as
        final_result1 = _mm_add_epi8(final_result1, adds_constants);

        // Interpolation condition:  IF result[_] <es THEN as ELIF result[_] >ew THAN aw ELSE am
        final_result1 = _mm_blendv_epi8(final_result1, _mm_set1_epi8(aw),  mask_ew);
        final_result1 = _mm_blendv_epi8(final_result1, _mm_set1_epi8(as),  mask_es);
        final_result1 = _mm_blendv_epi8(final_result1, _mm_set1_epi8(am),
                                        _mm_cmpeq_epi8(simd_gray, _mm_set1_epi8(em)));

        // Convert back to uint8_t and store the result
        _mm_storeu_si128((__m128i_u *)(gray_map + i), final_result1);
    }

    // Compute remaining bytes iteratively
    for(; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else if (gray_map[i] <= em)
            gray_map[i] = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
        else
            gray_map[i] = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
    }
}


void bilinear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                                uint8_t es, uint8_t as, uint8_t em,
                                uint8_t am, uint8_t ew, uint8_t aw) {
    for(size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else if (gray_map[i] < em) {
            uint8_t first_inter = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
            uint8_t second_inter = as + ((aw - as) / (ew - es)) * (gray_map[i] - es);
            gray_map[i] = (first_inter + second_inter) / 2;
        } else {
            uint8_t first_inter = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
            uint8_t second_inter = as + ((aw - as) / (ew - es)) * (gray_map[i] - es);
            gray_map[i] = (first_inter + second_inter) / 2;
        }
    }
}
