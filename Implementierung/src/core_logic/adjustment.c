//
// Created by Adem Trabelsi on 26.12.23.
//

#include "adjustment.h"



void levels_adjustment(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                       uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    
    img_to_gray_scale_SIMD(result, img, width, height, (float) a, (float) b, (float) c);
    
    int out_of_bound = is_out_of_bound(es, as, em, am, ew, aw);
    
    quadratic_interpolation_Newton_SIMD(result, width, height, (float) es, (float) as, (float) em,
                                  (float) am, (float) ew, (float) aw, out_of_bound);

}
void levels_adjustment_V1(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale(result, img, width, height, (float) a, (float) b, (float) c);
    
    int out_of_bound = is_out_of_bound(es, as, em, am, ew, aw);
    
    quadratic_interpolation_Gaussian(result, width, height, (float) es, (float) as, (float) em,
                                  (float) am, (float) ew, (float) aw, out_of_bound);
             
}

void levels_adjustment_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, (float) a, (float) b, (float) c);
    
    int out_of_bound = is_out_of_bound(es, as, em, am, ew, aw);
    
    quadratic_interpolation_Gaussian_SIMD(result, width, height, (float) es, (float) as, (float) em,
                                  (float) am, (float) ew, (float) aw, out_of_bound);
}

void levels_adjustment_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale(result, img, width, height, (float) a, (float) b, (float) c);
    
    int out_of_bound = is_out_of_bound(es, as, em, am, ew, aw);
    
    quadratic_interpolation_Newton_LUT(result, width, height, (float) es, (float) as, (float) em,
                                  (float) am, (float) ew, (float) aw, out_of_bound);

}

void levels_adjustment_V4(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, (float) a, (float) b, (float) c);
        
    linear_interpolation_SIMD(result, width, height, es, as, em, am, ew, aw);
}

void levels_adjustment_V5(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, (float) a, (float) b, (float) c);
        
    bilinear_interpolation_LUT(result, width, height, es, as, em, am, ew, aw);
                       
}

void levels_adjustment_V6(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, (float) a, (float) b, (float) c);
    
    int out_of_bound = is_out_of_bound(es, as, em, am, ew, aw);
    //TODO 
    quadratic_interpolation_BLagrange_LUT(result, width, height, es, as, em, am, ew, aw);

}

