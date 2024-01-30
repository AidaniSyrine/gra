//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <immintrin.h>

/// @brief Converts an RBG image to grayscale using a linear combination of the color channels.
/// Optimization level 0.
/// @param gray_map Pointer to the output grayscale image.
/// @param pix_map Pointer to the input RGB image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param a Weight for the red channel.
/// @param b Weight for the green channel.
/// @param c Weight for the blue channel.
void img_to_gray_scale(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                           float a, float b, float c);

/// @brief Converts an RBG image to grayscale using a linear combination of the color channels, optimized by SIMD.
/// Optimization level 1.
/// @param gray_map Pointer to the output grayscale image.
/// @param pix_map Pointer to the input RGB image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param a Weight for the red channel.
/// @param b Weight for the green channel.
/// @param c Weight for the blue channel.
void img_to_gray_scale_SIMD(uint8_t* gray_map, const uint8_t* pix_map, size_t width, size_t height,
                           float a, float b, float c);

