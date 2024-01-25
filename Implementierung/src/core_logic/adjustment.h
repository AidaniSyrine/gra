//
// Created by Adem Trabelsi on 26.12.23.
//
#pragma once

#include "grayscale.h"
#include "linear_interpolation.h"
#include "quadratic_interpolation.h"


/// @brief Main Implementation
/// @param img 
/// @param width 
/// @param height 
/// @param a 
/// @param b 
/// @param c 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
/// @param result 
void levels_adjustment(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                       uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);


/// @brief Trivial Implementation
/// @param img 
/// @param width 
/// @param height 
/// @param a 
/// @param b 
/// @param c 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
/// @param result 
void levels_adjustment_V1(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                           uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);


/// @brief Optimized trivial implementation (+SIMD)
/// @param img 
/// @param width 
/// @param height 
/// @param a 
/// @param b 
/// @param c 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
/// @param result 
void levels_adjustment_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

/// @brief Main Implementation (LUT: On Arm systems instead of SIMD)
/// @param img 
/// @param width 
/// @param height 
/// @param a 
/// @param b 
/// @param c 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
/// @param result 
void levels_adjustment_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

/// @brief Linear interpolation SIMD
/// @param img 
/// @param width 
/// @param height 
/// @param a 
/// @param b 
/// @param c 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
/// @param result 
void levels_adjustment_V4(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

/// @brief Bilinear interpolation 
/// @param img 
/// @param width 
/// @param height 
/// @param a 
/// @param b 
/// @param c 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
/// @param result 
void levels_adjustment_V5(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);
/// @brief BLagrange interpolation
/// @param img 
/// @param width 
/// @param height 
/// @param a 
/// @param b 
/// @param c 
/// @param es 
/// @param as 
/// @param em 
/// @param am 
/// @param ew 
/// @param aw 
/// @param result 
void levels_adjustment_V6(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);
                          