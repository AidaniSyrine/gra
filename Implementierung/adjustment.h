//
// Created by Adem Trabelsi on 26.12.23.
//
#pragma once


#include <time.h>
#include "grayscale.h"
#include "interpolation.h"

static inline double curtime(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;

}

// "main implemetation entry point"

// Linear +SIMD
void levels_adjustment(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                       uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);


// Linear -SIMD
void levels_adjustment_V1(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                           uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

// Bilinear
void levels_adjustment_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

// Quadratic LS -SIMD
void levels_adjustment_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

// Quadratic LS +SIMD
void levels_adjustment_V4(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

// Quadratic Newton
void levels_adjustment_V5(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

// Quadratic Barycentric Lagrange
void levels_adjustment_V5(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

