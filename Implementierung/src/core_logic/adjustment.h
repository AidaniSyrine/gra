//
// Created by Adem Trabelsi on 26.12.23.
//
#pragma once

#include "grayscale.h"
#include "linear_interpolation.h"
#include "quadratic_interpolation.h"

/// @brief Main version implementation using SIMD version of grayscale
/// conversion and of Newton quadratic interpolation for the tonal-correction
/// Optimisation level 3
/// @param img Pointer to the input RGB image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param a Weight for the red channel used in the grayscale conversion.
/// @param b Weight for the green channel used in the grayscale conversion.
/// @param c Weight for the blue channel used in the grayscale conversion.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel 
/// @param result Pointer to the output final image.
void levels_adjustment(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                       uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);


/// @brief Naive version as a comparaison implementation (1) using Gaussian 
/// Elimination for the quadratic interpolation for the tonal-correction
/// Optimization level 0 
/// @param img Pointer to the input RGB image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param a Weight for the red channel used in the grayscale conversion.
/// @param b Weight for the green channel used in the grayscale conversion.
/// @param c Weight for the blue channel used in the grayscale conversion.
/// @param es Input value for the Support point representing the black-pixel 
/// @param as Output value for the Support point representing the black-pixel 
/// @param em Input value for the Support point representing the middle-pixel 
/// @param am Output value for the Support point representing the middle-pixel 
/// @param ew Input value for the Support point representing the white-pixel 
/// @param aw Output value for the Support point representing the white-pixel 
/// @param result Pointer to the output final image.
void levels_adjustment_V1(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                           uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);


/// @brief Comparaison implementation (2) using Newton interpolation  
/// for the tonal-correction 
/// Optimization level 2 
/// @param img Pointer to the input RGB image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param a Weight for the red channel used in the grayscale conversion.
/// @param b Weight for the green channel used in the grayscale conversion.
/// @param c Weight for the blue channel used in the grayscale conversion.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel 
/// @param result Pointer to the output final image.
void levels_adjustment_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);

/// @brief implementation using SIMD version of grayscale conversion
/// and of Newton quadratic interpolation for the tonal-correction
/// @param img Pointer to the input RGB image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param a Weight for the red channel used in the grayscale conversion.
/// @param b Weight for the green channel used in the grayscale conversion.
/// @param c Weight for the blue channel used in the grayscale conversion.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel 
/// @param result Pointer to the output final image.
void levels_adjustment_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c,
                          uint8_t es, uint8_t as, uint8_t em, uint8_t am, uint8_t ew, uint8_t aw, uint8_t* result);
