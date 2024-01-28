//
// Created by Adem Trabelsi on 26.12.23.
//

#include "adjustment.h"

//TODO 

void levels_adjustment(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                       uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    
    quadratic_interpolation_newton_SIMD(result, width, height,  es, as, em,
                                 am, ew, aw, 1);

}
void levels_adjustment_V1(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
     img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    
     quadratic_interpolation_newton_SIMD(result, width, height,  es, as, em,
                                 am, ew, aw, 1);
}

void levels_adjustment_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    
    quadratic_interpolation_newton_SIMD(result, width, height,  es, as, em,
                                 am, ew, aw, 1);
}

void levels_adjustment_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    
    quadratic_interpolation_newton_SIMD(result, width, height,  es, as, em,
                                 am, ew, aw, 1);
}

void levels_adjustment_V4(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    
    quadratic_interpolation_newton_SIMD(result, width, height,  es, as, em,
                                 am, ew, aw, 1);
}

void levels_adjustment_V5(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
  img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    
    quadratic_interpolation_newton_SIMD(result, width, height,  es, as, em,
                                 am, ew, aw, 1);
}

void levels_adjustment_V6(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
   img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    
    quadratic_interpolation_newton_SIMD(result, width, height,  es, as, em,
                                 am, ew, aw, 1);
}

