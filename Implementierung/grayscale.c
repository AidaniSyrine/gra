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
    size_t num_pix = width * height * 3 * sizeof(uint8_t) ;
    if (!gray_map) {
        // Memory allocation failed
        return ;
    }

    const __m128 a_coeff = _mm_set1_ps(a);
     
    const __m128 b_coeff = _mm_set1_ps(b);
 
    const __m128 c_coeff = _mm_set1_ps(c);
     
    const __m128 div = _mm_set1_ps(a+b+c);
    
    size_t i,s; 

    for (; i*12<=num_pix-12;i+=12, s+=4){

        // load R from 4 pixels
        __m128 a_col = _mm_set_ps(pix_map[i+9],pix_map[i+6],pix_map[i+3],pix_map[i]);
        // load G from 4 pixels
        __m128 b_col =  _mm_set_ps(pix_map[i+10],pix_map[i+7],pix_map[i+4],pix_map[i+1]);
        // load B from 4 pixels
        __m128 c_col =  _mm_set_ps(pix_map[i+11],pix_map[i+8],pix_map[i+5],pix_map[i+2]);
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

}

