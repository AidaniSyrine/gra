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
uint8_t* linear_interpolation(uint8_t* pix_map, int width, int height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw);


/**
 *
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
uint8_t* quadratic_interpolation(uint8_t* pix_map, int width, int height,
                                 uint8_t es, uint8_t as, uint8_t em,
                                 uint8_t am, uint8_t ew, uint8_t aw);
