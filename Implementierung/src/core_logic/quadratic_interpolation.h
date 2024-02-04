//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <math.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>

#define ROUNDING 0.000001

/// @brief Solve a system of linear equations using Gauss-Jordan Algorithm
/// @param num_rows Number of rows of the Matrix.
/// @param num_cols  Number of columns of the Matrix.
/// @param matrix Transformation-matrix of the system of linear equations.
void row_reduced_echelon_form(int num_rows, int num_cols, float matrix[num_rows][num_cols]); 

/// @brief Evaluate the interpolation polynomial using rref
/// @param gray_map Pointer to the grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel 
void quadratic_interpolation_gaussian(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw);

/// @brief Evaluate the interpolation polynomial using rref 
/// optimization using Look-up-Table
/// @param gray_map Pointer to the grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel
void quadratic_interpolation_gaussian_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw);

/// @brief Evaluate the interpolation polynomial using rref 
/// optimization using SIMD
/// @param gray_map Pointer to the grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel
void quadratic_interpolation_gaussian_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw);

/// @brief Evaluate the interpolation polynomial using Lagrange basispolynomials
/// @param gray_map Pointer to the grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel 
void quadratic_interpolation_lagrange(uint8_t* gray_map, size_t width, size_t height,
                                       float es, float as, float em,
                                       float am, float ew, float aw);

/// @brief Evaluate the interpolation polynomial using barycentic Lagrange interpolation
/// @param gray_map Pointer to the grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel 
void quadratic_interpolation_blagrange(uint8_t* gray_map, size_t width, size_t height,
                                       float es, float as, float em,
                                       float am, float ew, float aw);

/// @brief Evaluate the interpolation polynomial using Newton basispolynomials
/// @param gray_map Pointer to the grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel 
void quadratic_interpolation_newton(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw);


/// @brief Evaluate the interpolation polynomial using Newton basispolynomials
/// optimization using SIMD
/// @param gray_map Pointer to the grayscale image.
/// @param width Width of the image.
/// @param height Height of the image.
/// @param es Input value for the support point representing the black-pixel 
/// @param as Output value for the support point representing the black-pixel 
/// @param em Input value for the support point representing the middle-pixel 
/// @param am Output value for the support point representing the middle-pixel 
/// @param ew Input value for the support point representing the white-pixel 
/// @param aw Output value for the support point representing the white-pixel
void quadratic_interpolation_newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                        float es, float as, float em,
                                        float am, float ew, float aw); 
                                        