//
// Created by Adem Trabelsi on 21.12.23.
//

#pragma once

#include <inttypes.h>
#include <stdlib.h>



/**
 *
 * @param pix_map
 * @param width
 * @param height
 * @return
 */
void linear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw);


/**
 *
 * @param gray_map
 * @param width
 * @param height
 * @param es
 * @param as
 * @param em
 * @param am
 * @param ew
 * @param aw
 * @return
 */
void bilinear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                            uint8_t es, uint8_t as, uint8_t em,
                            uint8_t am, uint8_t ew, uint8_t aw);


/**
 *  Gaussian Elimination
 * @param pix_map
 * @param width
 * @param height
 * @param es
 * @param as
 * @param em
 * @param am
 * @param ew
 * @param aw
 * @return
 */
void quadratic_interpolation_LS (uint8_t* gray_map, size_t width, size_t height,
                                 uint8_t es, uint8_t as, uint8_t em,
                                 uint8_t am, uint8_t ew, uint8_t aw);
/**
 *
 * @param gray_map
 * @param width
 * @param height
 * @param es
 * @param as
 * @param em
 * @param am
 * @param ew
 * @param aw
 */
void quadratic_interpolation_Lagrange(uint8_t* gray_map, size_t width, size_t height,
                                      uint8_t es, uint8_t as, uint8_t em,
                                      uint8_t am, uint8_t ew, uint8_t aw);

/**
 *
 * @param gray_map
 * @param width
 * @param height
 * @param es
 * @param as
 * @param em
 * @param am
 * @param ew
 * @param aw
 */
void quadratic_interpolation_Newton(uint8_t* gray_map, size_t width, size_t height,
                                     uint8_t es, uint8_t as, uint8_t em,
                                     uint8_t am, uint8_t ew, uint8_t aw);








