//
// Created by Adem Trabelsi on 21.12.23.
//

#include "correction.h"




uint8_t* linear_interpolation(uint8_t* pix_map, int width, int height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw)
{
    return NULL;
}


uint8_t* quadratic_interpolation(uint8_t* pix_map, int width, int height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw) {

    uint8_t* cor_img = (uint8_t*) malloc(width * height * sizeof(uint8_t));

       for (size_t i = 0; i < width * height; ++i) {

        if (pix_map[i] == es ) {
            cor_img[i] = as;
        }
        if (pix_map[i] == ew ) {
            cor_img[i] = aw;
        }
        if (pix_map[i] == em ) {
            cor_img[i] = am;
        }
            
        double matrix[3][4] = {
            {em*em, em, 1, am} , 
            {ew*ew, ew, 1, aw},
            {es*es, es, 1, as}
        };

        double s1 , s2, s3;

    solutions( matrix, &s1,  &s2, &s3);

    return NULL;
}

int solutions( double matrix[3][4], double* s1, double* s2, double* s3 ) {
    for (int i = 0; i < 2; i++) {
        for (int k = i + 1; k < 3; k++) {
            double factor = matrix[k][i] / matrix[i][i];
            for (int j = i; j <= 3; j++) {
                matrix[k][j] -= factor * matrix[i][j];
            }
        }
    }
     for (int i = 0; i < 3; i++) {
        for (int k = 0 ; k < 4; k++) {
             printf("%f\n", matrix[i][k]);
        }
     }

     for (int i = 2; i >= 0; i--) {
        for (int j = i + 1; j < 3; j++) {
            matrix[i][3] -= matrix[i][j] * matrix[j][3];
        }
        matrix[i][3] /= matrix[i][i];
         printf("%f\n", matrix[i][3]);
    }
    
    *s1 =  matrix[0][3]; 
    *s2 =  matrix[1][3]; 
    *s3 =  matrix[2][3]; 
    
return 0;
}

