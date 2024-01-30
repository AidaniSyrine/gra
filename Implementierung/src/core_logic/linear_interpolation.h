//
// Created by Adem Trabelsi on 12.01.24.
//
#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>


/// @brief Apply linear interpolation to a grayscale image.
/// @param gray_map  Pointer to the input and output grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the Support point representing the black-pixel. 
/// @param as Output value for the Support point representing the black-pixel. 
/// @param em Input value for the Support point representing the middle-pixel. 
/// @param am Output value for the Support point representing the middle-pixel. 
/// @param ew Input value for the Support point representing the white-pixel. 
/// @param aw Output value for the Support point representing the white-pixel. 
void linear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw);

/// @brief Apply linear interpolation optimized by SIMD to a grayscale image.
/// @param gray_map  Pointer to the input and output grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the Support point representing the black-pixel. 
/// @param as Output value for the Support point representing the black-pixel. 
/// @param em Input value for the Support point representing the middle-pixel. 
/// @param am Output value for the Support point representing the middle-pixel. 
/// @param ew Input value for the Support point representing the white-pixel. 
/// @param aw Output value for the Support point representing the white-pixel. 
void linear_interpolation_SIMD(uint8_t* gray_map, size_t width, size_t height,
                               uint8_t es, uint8_t as, uint8_t em,
                               uint8_t am, uint8_t ew, uint8_t aw);

/// @brief Apply bilinear interpolation to a grayscale image.
/// @param gray_map  Pointer to the input and output grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the Support point representing the black-pixel. 
/// @param as Output value for the Support point representing the black-pixel.
/// @param em Input value for the Support point representing the middle-pixel. 
/// @param am Output value for the Support point representing the middle-pixel. 
/// @param ew Input value for the Support point representing the white-pixel.
/// @param aw Output value for the Support point representing the white-pixel. 
void bilinear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                            uint8_t es, uint8_t as, uint8_t em,
                            uint8_t am, uint8_t ew, uint8_t aw);

            