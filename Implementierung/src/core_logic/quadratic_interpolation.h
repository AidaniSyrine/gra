//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once
#include <inttypes.h>
#include <stdlib.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>


/**
 *
 * @param pix_map
 * @param width
 * @param height
 * @return
 */



void quadratic_interpolation_LS(uint8_t* gray_map, size_t width, size_t height,
                                 uint8_t es, uint8_t as, uint8_t em,
                                 uint8_t am, uint8_t ew, uint8_t aw, int outbound);

void quadratic_interpolation_LS_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw);

void quadratic_interpolation_LS_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                  uint8_t es, uint8_t as, uint8_t em,
                                  uint8_t am, uint8_t ew, uint8_t aw, int outbound);

void quadratic_interpolation_BLagrange(uint8_t* gray_map, size_t width, size_t height,
                                      uint8_t es, uint8_t as, uint8_t em,
                                      uint8_t am, uint8_t ew, uint8_t aw);

void quadratic_interpolation_BLagrange_LUT(uint8_t* gray_map, size_t width, size_t height,
                                       uint8_t es, uint8_t as, uint8_t em,
                                       uint8_t am, uint8_t ew, uint8_t aw);

void quadratic_interpolation_Newton(uint8_t* gray_map, size_t width, size_t height,
                                     uint8_t es, uint8_t as, uint8_t em,
                                     uint8_t am, uint8_t ew, uint8_t aw);

void quadratic_interpolation_Newton_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw);

void quadratic_interpolation_Newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw);






