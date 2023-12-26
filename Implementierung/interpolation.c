//
// Created by Adem Trabelsi on 21.12.23.
//
#include "interpolation.h"

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
    for(size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] <= em) {
            uint8_t first_inter = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
            uint8_t second_inter = as + ((aw - as) / (ew - es)) * (gray_map[i] - es);
            gray_map[i] = (first_inter + second_inter) / 2;
        } else {
            uint8_t first_inter = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
            uint8_t second_inter = as + ((aw - as) / (ew - es)) * (gray_map[i] - es);
            gray_map[i] = (first_inter + second_inter) / 2;
        }
    }
}


//SBeanch2 "+helper methods"??
void quadratic_interpolation_GaussianElimination(uint8_t* gray_map, size_t width, size_t height,
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

//Double ??
void quadratic_interpolation_BLagrange(uint8_t* gray_map, size_t width, size_t height,
                                       uint8_t es, uint8_t as, uint8_t em,
                                       uint8_t am, uint8_t ew, uint8_t aw) {
    // Just for code readability and convenience
    uint8_t x[] = {es, em, ew};
    uint8_t y[] = {as, am, aw};
    float bary_weights[3], numerator, denomirator;
    int product;

    // Compute barycentric weights
    for (size_t i = 0; i < sizeof (bary_weights) / sizeof (*bary_weights); i++) {
        product = 1;
        for (size_t j = 0; j < sizeof (x) / sizeof (*x); j++) {
            if (i == j) continue;
            product *= x[i] - x[j];
        }
        bary_weights[i] = 1.f / (float) product;
    }

    // Barycentric lagrange interpolation
    for (size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else
        {
            numerator = denomirator = 0;
            for (size_t j = 0; j < sizeof (bary_weights) / sizeof (*bary_weights); j++) {
                numerator += (bary_weights[j] / (float) (gray_map[i] - x[j])) * (float) y[j];
                denomirator += (bary_weights[j]) / (float) (gray_map[i] - x[j]);
            }
            gray_map[i] = (uint8_t) (numerator / denomirator);
        }
    }
}


void quadratic_interpolation_Newton(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw) {

    // Just for code readability and convenience
    uint8_t x[] = {es, em, ew};
    uint8_t diff_table[][3] = {{as}, {am}, {aw}};

    // Compute Newton coeffs
    for (size_t i = 1; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (size_t j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
            diff_table[j][i]  = (diff_table[j + 1][i -1] - diff_table[j][i - 1]) / (x[i +j] - x[j]);
        }
    }

    // Newton interpolation
    for(size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else gray_map[i] = diff_table[0][0] + diff_table[0][1] * (gray_map[i] - x[0])
                + diff_table[0][2] * (gray_map[i] - x[0]) * (gray_map[i] - x[1]);
    }
}


