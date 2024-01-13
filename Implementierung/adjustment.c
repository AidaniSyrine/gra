//
// Created by Adem Trabelsi on 26.12.23.
//

#include "adjustment.h"
#include "io_operations.h"


void levels_adjustment(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                       uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    int outbound = isoutbound(es, as, em, am, ew, aw);
    if (outbound) quadratic_interpolation_LS_SIMD(result, width, height, es, as, em, am, ew, aw, outbound);
    else linear_interpolation_SIMD(result, width, height, es, as, em, am, ew, aw);
}

void levels_adjustment_V1(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    linear_interpolation(result, width, height, es, as, em, am, ew, aw);
}

void levels_adjustment_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    bilinear_interpolation(result, width, height, es, as, em, am, ew, aw);
}

void levels_adjustment_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    int outbound = isoutbound(es, as, em, am, ew, aw);
    quadratic_interpolation_LS(result, width, height, es, as, em, am, ew, aw, outbound);
}

void levels_adjustment_V4(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    int outbound = isoutbound(es, as, em, am, ew, aw);
    quadratic_interpolation_LS_SIMD(result, width, height, es, as, em, am, ew, aw, outbound);
}

void levels_adjustment_V5(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    quadratic_interpolation_Newton(result, width, height, es, as, em, am, ew, aw);
}

void levels_adjustment_V6(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result) {
    img_to_gray_scale_SIMD(result, img, width, height, a, b, c);
    quadratic_interpolation_BLagrange(result, width, height, es, as, em, am, ew, aw);
}

