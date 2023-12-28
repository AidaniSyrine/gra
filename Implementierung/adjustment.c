//
// Created by Adem Trabelsi on 21.12.23.
//
#include <stdio.h>
#include "adjustment.h"

// Next Update SIMD
void linear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw)
{
   for(size_t i = 0; i < width*height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
       else if (gray_map[i] < em)
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
    __m128i simd_aw = _mm_set1_epi8(aw);

    __m128i simd_factor1 = _mm_set1_epi8((am - as) / (em - es));
    __m128i simd_factor2 = _mm_set1_epi8((u_int8_t)((aw - am) / (ew - em)));
 
    size_t size = width * height;
    size_t i =0;
    for (i=0; i <size-size%16; i += 16) { 
        //loading 16 bytes 
        __m128i simd_gray = _mm_loadu_si128((__m128i*)(gray_map+i));
        
        // change bytes <es to 0
        __m128i mask_es = _mm_cmpgt_epi16(_mm_unpacklo_epi8(simd_gray, _mm_setzero_si128()), simd_es);
        mask_es = _mm_packs_epi16 (mask_es,_mm_cmpgt_epi16(_mm_unpackhi_epi8(simd_gray, _mm_setzero_si128()), simd_es));
        mask_es = _mm_andnot_si128(mask_es, _mm_set1_epi8(0xff));
        __m128i result = _mm_blendv_epi8(simd_gray, _mm_set1_epi8(0),  mask_es);
        
        // change bytes >ew to 0
         __m128i mask_ew = _mm_cmplt_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()), simd_ew);
        mask_ew = _mm_packs_epi16 (mask_ew,_mm_cmplt_epi16(_mm_unpackhi_epi8(result, _mm_setzero_si128()), simd_ew));
        mask_ew = _mm_andnot_si128(mask_ew, _mm_set1_epi8(0xff));
        result = _mm_blendv_epi8(result, _mm_set1_epi8(0),  mask_ew);

        // register of adequat factors
         __m128i mask_ltem = _mm_cmplt_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()), simd_em);
        mask_ltem = _mm_packs_epi16 (mask_ltem,_mm_cmplt_epi16(_mm_unpackhi_epi8(result, _mm_setzero_si128()), simd_em));
        
        __m128i factors = _mm_and_si128(mask_ltem, simd_factor1 ) ;
        __m128i mask_getem =_mm_andnot_si128(mask_ltem, _mm_set1_epi8(0xff));

        factors = _mm_or_si128(factors, _mm_and_si128(mask_getem, simd_factor2 ) );
        
        // register of adequat constant to be added
        __m128i addconstants = _mm_and_si128(mask_ltem, simd_as ) ;
        addconstants = _mm_or_si128(addconstants, _mm_and_si128(mask_getem, simd_am ) );

        //register of adequat constant to be substructed
        __m128i subconstants = _mm_and_si128(mask_ltem, _mm_set1_epi8(es) ) ;
        subconstants = _mm_or_si128(subconstants, _mm_and_si128(mask_getem, _mm_set1_epi8(em) ) );

        // substraction
        result= _mm_sub_epi8 (result,subconstants);

        // first 8 bytes
        __m128i final_result1 = _mm_maddubs_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()) , _mm_unpacklo_epi8(factors, _mm_setzero_si128()));
        __m128i final_result2 = _mm_maddubs_epi16(_mm_unpackhi_epi8(result, _mm_setzero_si128()) , _mm_unpackhi_epi8(factors, _mm_setzero_si128()));
        final_result1 = _mm_packus_epi16 (final_result1,final_result2);

        // add constant 
        final_result1 = _mm_add_epi8(final_result1, addconstants);
        
        // if <es return as ; if >ew return aw ; if ==em return am
        final_result1 = _mm_blendv_epi8(final_result1, _mm_set1_epi8(aw),  mask_ew);
        final_result1 = _mm_blendv_epi8(final_result1, simd_as,  mask_es);
        final_result1 = _mm_blendv_epi8(final_result1, simd_am,  _mm_cmpeq_epi8(simd_gray, _mm_set1_epi8(em)));

        // Convert back to uint8_t and store the result
        _mm_storeu_si128((gray_map + i) ,final_result1);
    }

    //complete remaining pixels iteratively
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
}

// Next Update SIMD
void quadratic_interpolation_LS(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw) {

    // Solving LS using Gaussian Elimination
    float numerator = (as - aw) * (es - ew) - (as - aw) * (es - em);
    float denominator = (es * es - em * em) * (es - ew) - (es * es-ew * ew) * (es - em);

    float s1 = numerator / denominator;

    float numerator1 = (as - am) * (es - ew) - s1 * (es * es - em * em) * (es - ew);
    float denominator1 = (es - em) * (es - ew);

    float s2 = numerator1 / denominator1;
    
    float s3 = as - s1 * es * es - s2 * es;    

    //calculating interpolation value  
    for (size_t i = 0; i< width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else {
            float tmp = s1 * gray_map[i] * gray_map[i];
            float tmp1 = s2*gray_map[i];
            gray_map[i] = tmp+tmp1+s3;
        }
    }
}

void quadratic_interpolation_LS_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                  uint8_t es, uint8_t as, uint8_t em,
                                  uint8_t am, uint8_t ew, uint8_t aw)
{
    //setting registers with constants
    __m128 esVec = _mm_set_ps1((float)es);
    __m128 asVec = _mm_set_ps1((float)as);
    __m128 ewVec = _mm_set_ps1((float)ew);
    __m128 awVec = _mm_set_ps1((float)aw);
    __m128 emVec = _mm_set_ps1((float)em);
    __m128 amVec = _mm_set_ps1((float)am);

    // Solving LS using Gaussian Elimination
    float numerator = (as - aw) * (es - ew) - (as - aw) * (es - em);
    float denominator = (es * es - em * em) * (es - ew) - (es * es-ew * ew) * (es - em);

    float s1 = numerator / denominator;
    __m128 s1Vec =_mm_set_ps1(s1);


    float numerator1 = (as - am) * (es - ew) - s1 * (es * es - em * em) * (es - ew);
    float denominator1 = (es - em) * (es - ew);

    float s2 = numerator1 / denominator1;
    __m128 s2Vec =_mm_set_ps1(s2);

    float s3 = as - s1 * es * es - s2 * es;
    __m128 s3Vec =_mm_set_ps1(s3);

    size_t size = width * height;

    size_t i;
    for (i = 0; i < size - (size % 4); i += 4){
        __m128 grayMapVec= _mm_set_ps((float)gray_map[i+3],(float)gray_map[i+2],
                                     (float)gray_map[i+1],(float)gray_map[i]);
        
        __m128 condition1 = _mm_cmple_ps(grayMapVec, esVec);
         
        __m128 condition2 = _mm_cmpge_ps(grayMapVec, ewVec);
        
        __m128 condition3 = _mm_and_ps(_mm_cmpge_ps(grayMapVec, esVec), _mm_cmple_ps(grayMapVec, ewVec));

        __m128 potentialResult = _mm_add_ps(_mm_mul_ps(s1Vec, _mm_mul_ps(grayMapVec, grayMapVec)),
                                            _mm_add_ps(_mm_mul_ps(s2Vec, grayMapVec), s3Vec));

        __m128 result = _mm_blendv_ps( _mm_blendv_ps(potentialResult,awVec,condition2),asVec,condition1);
       
        __m128i pack1 = _mm_packus_epi16(_mm_cvttps_epi32(result),_mm_cvttps_epi32(result));
        __m128i pack =  _mm_packus_epi16(pack1,pack1);
        
        _mm_storeu_si32((__m128i*)&gray_map[i], pack);       
    }

    // calculating remainig pixels iteratively
    for(; i < width * height; i++){
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else {
            float tmp = s1 * gray_map[i] * gray_map[i];
            float tmp1 = s2*gray_map[i];
            gray_map[i] = tmp+tmp1+s3;
        }
    }
}


void quadratic_interpolation_Lagrange(uint8_t* gray_map, size_t width, size_t height,
                                       uint8_t es, uint8_t as, uint8_t em,
                                       uint8_t am, uint8_t ew, uint8_t aw) {
    // Just for code readability and convenience
    uint8_t x[] = {es, em, ew};
    uint8_t y[] = {as, am, aw};
    uint8_t polynome;

    for(size_t z = 0; z < width * height; z++) {
        for(int i = 0; i < 3; i++) {
            polynome = 1;
            for(int j = 0; j < 3; j++) {
                // Lagrange polynomial
                if(i == j) continue;
                polynome = (polynome / (x[i]  - x[j])) * (gray_map[z] - x[j]) ;
            }
            gray_map[z] += (uint8_t) y[i] * polynome;
        }
    }
}


void quadratic_interpolation_Newton(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw) {

    // Just for code readability and convenience
    uint8_t x[] = {es, em, ew};
    uint8_t diff_table[][3] = {{as}, {am}, {aw}};
    
    // Compute Newton coeff
    for (int i = 0; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (int j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
           diff_table[i][j]  = (diff_table[i + 1][j -1] - diff_table[i][j - 1]) / (x[i +j] - x[i]);
        }
    }
    for(size_t i = 0; i < width * height; i++) {
        gray_map[i] = diff_table[0][0] + diff_table[0][1] * (gray_map[i] - x[0])
                + diff_table[0][2] * (gray_map[i] - x[0]) * (gray_map[i] - x[1]);
    }








    for (int i = 0; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for(int j = 0; j < sizeof (*diff_table) / sizeof (**diff_table); j++) {
            printf("%hhu ", diff_table[i][j]);
        }
        puts("");
    }




}


// first change in Lagrange

