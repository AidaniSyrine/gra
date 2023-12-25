//
// Created by Adem Trabelsi on 21.12.23.
//

#include "grayscale.h"


uint8_t* img_to_grey_scale(uint8_t* pix_map, int width, int height, float a, float b, float c) {
    size_t num_pix = width * height * 3 * sizeof(uint8_t) ;
    uint8_t* gray_map = (uint8_t*) malloc(  width * height * sizeof(uint8_t) );
    if (!gray_map) {
        // Memory allocation failed
        return NULL;
    }

    const __m128 a_coeff = _mm_set_ps(a,a,a,a);
    const __m128 b_coeff = _mm_set_ps(b,b,b,b);
    const __m128 c_coeff = _mm_set_ps(c,c,c,c);
    const __m128 div = _mm_set_ps(a+b+c,a+b+c,a+b+c,a+b+c);
    
    size_t i,s; 

    for (; i*12<=num_pix-12;i+=12, s+=4){

        // load R from 4 pixels
        __m128 a_col = _mm_set_ps(pix_map[i],pix_map[i+3],pix_map[i+6],pix_map[i+9]);
        // load G from 4 pixels
        __m128 b_col = _mm_set_ps(pix_map[i+1],pix_map[i+4],pix_map[i+7],pix_map[i+10]);
        // load B from 4 pixels
        __m128 c_col = _mm_set_ps(pix_map[i+2],pix_map[i+5],pix_map[i+8],pix_map[i+11]);
        // multiply each color with the coeff 
        a_col = _mm_mul_ps(a_col, a_coeff);
        b_col = _mm_mul_ps(b_col, b_coeff);
        c_col = _mm_mul_ps(c_col, c_coeff);
        // divide the sum by (a+b+c)
       __m128 result  = _mm_div_ps(_mm_add_ps(a_col, _mm_add_ps(b_col, c_col)), div);
       //store the 4 calculated values in grey_map
       
       for(size_t k=0; k<4 && s+k<width * height ; k++){
        gray_map[s+k] = (uint8_t) result[k];

       }
       
    }
    // calculate the grey value iteratively
    for (; s<width*height;s++){
         gray_map[s]=(a*pix_map[3*s]+b*pix_map[3*s+1]+c*pix_map[3*s+2])/(a+b+c);
    }
    return gray_map;
}
