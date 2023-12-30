//
// Created by Adem Trabelsi on 26.12.23.
//
#pragma once

#include "grayscale.h"
#include "interpolation.h"


// "main implemetation entry point"

// Linear +SIMD
/**
 * @Description:
 * @param img
 * @param width
 * @param height
 * @param a
 * @param b
 * @param c
 * @param es
 * @param as
 * @param em
 * @param am
 * @param ew
 * @param aw
 * @param result
 */
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

void levels_adjustment_V6(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);