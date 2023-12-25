//
// Created by Adem Trabelsi on 21.12.23.
//
#include <stdio.h>
#include "adjustment.h"

// Next Update SIMD
void linear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw)
{
   for(size_t i = 0; i < width * height; i++) {
       if (gray_map[i] <= es) gray_map[i] = as;
       else if (gray_map[i] >= ew) gray_map[i] = aw;
       else if (gray_map[i] <= em)
           gray_map[i] = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
       else
           gray_map[i] = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
   }
}


void bilinear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                                uint8_t es, uint8_t as, uint8_t em,
                                uint8_t am, uint8_t ew, uint8_t aw) {
}

// Next Update SIMD
void quadratic_interpolation_LS(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw) {

    // Solving LS using Gaussian Elimination
    int s1 = ((as - aw) * (es - ew) - (as - aw) * (es - em))
            / ((es * es - em * em) * (es - ew) - (es * es-ew * ew) * (es - em));
    int s2 = ((as - am) * (es - ew) - s1 * (es * es - em * em) * (es - ew))
            / ((es - em) * (es - ew));
    int s3 = as - s1 * es * es - s2 * es;

    for (size_t i = 0; i< width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else gray_map[i] = s1 * gray_map[i] * gray_map[i] + s2 * gray_map[i] + s3;
    }
}

void quadratic_interpolation_Lagrange(uint8_t* gray_map, size_t width, size_t height,
                                       uint8_t es, uint8_t as, uint8_t em,
                                       uint8_t am, uint8_t ew, uint8_t aw) {
    // Just for code readability and convenience
    uint8_t x[] = {es, em, ew};
    uint8_t y[] = {as, am, aw};
    uint8_t polynome;

    for(size_t z = 0; z < width * height; z++) {
        for(int i = 0; i < 3; i++) {
            polynome = 1;
            for(int j = 0; j < 3; j++) {
                // Lagrange polynomial
                if(i == j) continue;
                polynome = (polynome / (x[i]  - x[j])) * (gray_map[z] - x[j]) ;
            }
            gray_map[z] += (uint8_t) y[i] * polynome;
        }
    }
}


void quadratic_interpolation_Newton(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw) {

    // Just for code readability and convenience
    uint8_t x[] = {es, em, ew};
    uint8_t diff_table[][3] = {{as}, {am}, {aw}};
    
    // Compute Newton coeff
    for (int i = 0; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (int j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
           diff_table[i][j]  = (diff_table[i + 1][j -1] - diff_table[i][j - 1]) / (x[i +j] - x[i]);
        }
    }
    for(size_t i = 0; i < width * height; i++) {
        gray_map[i] = diff_table[0][0] + diff_table[0][1] * (gray_map[i] - x[0])
                + diff_table[0][2] * (gray_map[i] - x[0]) * (gray_map[i] - x[1]);
    }








    for (int i = 0; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for(int j = 0; j < sizeof (*diff_table) / sizeof (**diff_table); j++) {
            printf("%hhu ", diff_table[i][j]);
        }
        puts("");
    }




}


// first change in Lagrange

