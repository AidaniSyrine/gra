//
// Created by Adem Trabelsi on 21.12.23.
//
#include "quadratic_interpolation.h"

#include <stdio.h>


//---------------------------------Gaussian--------------------------------------

void row_reduced_echelon_form(int num_rows, int num_cols, float matrix[num_rows][num_cols]) {
    int k, pivot;
    float max;
    for(int i = 0, j = 0; i < num_rows && j < num_cols; i++) {
        // Partial Pivoting
        max = fabsf(matrix[i][j]);
        pivot = i;
        for (k = i; k < num_rows; k++) {
            if (fabsf(matrix[k][j]) > max) {
                pivot = k;
                max = fabsf(matrix[k][j]);
            }
        }
        if (max < ROUNDING) {
            j++;
            continue;
        }
        // Swap pivot
        float tmp_vector[num_cols];
        if (pivot != i) {
            for (k = 0; k < num_cols; k++) {
                tmp_vector[k] = matrix[pivot][k];
                matrix[pivot][k] = matrix[i][k];
                matrix[i][k] = tmp_vector[k];
            }
        }
        // One in pivot
        for (k = 0; k < num_cols; k++) {
            if (k != j)
                matrix[i][k] *=  1 / matrix[i][j];
        }
        matrix[i][j] *= 1 / matrix[i][j];
        // Zeros on columns under/below pivot
        for (k = 0; k < num_rows; k++) {
            if (k != i) {
                for (int y = 0; y < num_cols; y++) {
                    if (y != j)
                        matrix[k][y] += (-matrix[k][j]) * matrix[i][y];
                }
                matrix[k][j] += -(matrix[k][j] * matrix[i][j]);
            }
        }
        j++;
    }
}

void quadratic_interpolation_gaussian(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw) {

    // Solving LS: Gaussian-Jordan Elimination + Partial Pivoting
    float augmented_matrix[][4] = {
        {es * es, es, 1,     as}, 
        {em * em, em, 1,     am}, 
        {ew * ew, ew, 1,     aw},
    };
    row_reduced_echelon_form(3, 4, augmented_matrix);
    float a = augmented_matrix[0][3]; 
    float b = augmented_matrix[1][3];
    float c = augmented_matrix[2][3];

    for (size_t i = 0; i < width*height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else {
               float result = a * gray_map[i] * gray_map[i] + b * gray_map[i] + c; 
               if (result > aw) gray_map[i] = aw; 
               else if (result < as) gray_map[i] = as; 
               else
               gray_map[i] = result;
        }
    }
}

//TODO 
void quadratic_interpolation_gaussian_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw, int outbound) {

    //setting registers with constants
    __m128 esVec = _mm_set_ps1(es);
    __m128 asVec = _mm_set_ps1(as);
    __m128 ewVec = _mm_set_ps1(ew);
    __m128 awVec = _mm_set_ps1(aw);
    __m128 emVec = _mm_set_ps1(em);
    __m128 amVec = _mm_set_ps1(am);

    // Solving LS using Gaussian Elimination
    float s[] = {0, 0, 0};

    uint8_t intersect_aw = ew;
    uint8_t intersect_as = es;
    if((s[0] < 0) && outbound) {
        intersect_aw = (uint8_t) ((- s[1] / (2*s[0])) - sqrt( ((aw - s[2]) /s[0]) + (s[1] * s[1] / (4 * s[0] * s[0]))));
    }
    if((s[0] > 0) && outbound) {
        intersect_as = (uint8_t) ((- s[1] / (2*s[0])) + sqrt( ((as - s[2]) / s[0]) + (s[1] * s[1] / (4 * s[0] * s[0]))));
    }

    __m128 s1Vec =_mm_set_ps1(s[0]);
    __m128 s2Vec =_mm_set_ps1(s[1]);
    __m128 s3Vec =_mm_set_ps1(s[2]);

    __m128 inter_aw =_mm_set_ps1(intersect_aw);
    __m128 inter_as =_mm_set_ps1(intersect_as);

    __m128 clampvec;
    if((s[0] < 0) && outbound) {
            clampvec = awVec;
    }
    if((s[0] > 0) && outbound) {
            clampvec = asVec;
    }


    size_t size = width * height;
    size_t i;
    for (i = 0; i < size - (size % 4); i += 4){
        __m128 grayMapVec= _mm_set_ps((float)gray_map[i+3],(float)gray_map[i+2],
                                      (float)gray_map[i+1],(float)gray_map[i]);
        __m128 condition1 = _mm_cmple_ps(grayMapVec, esVec);
        __m128 condition2 = _mm_cmpge_ps(grayMapVec, ewVec);
        __m128 condition3 = _mm_cmpeq_ps(grayMapVec, emVec);
        
        __m128 clampmask;
        if((s[0] < 0) && outbound) {
            clampmask = _mm_cmpgt_ps(grayMapVec, inter_aw);
        }
        if((s[0] > 0) && outbound) {
            clampmask = _mm_cmple_ps(grayMapVec, inter_as);  
        }
        __m128 potentialResult = _mm_add_ps(_mm_mul_ps(s1Vec, _mm_mul_ps(grayMapVec, grayMapVec)),
                                            _mm_add_ps(_mm_mul_ps(s2Vec, grayMapVec), s3Vec));
        __m128 result = _mm_blendv_ps( _mm_blendv_ps(potentialResult,awVec,condition2),asVec,condition1);
        result = _mm_blendv_ps(result,amVec,condition3);
        if (outbound)  result = _mm_blendv_ps(result,clampvec,clampmask);

        __m128i pack1 = _mm_packus_epi32(_mm_cvttps_epi32(result),_mm_setzero_si128());
        __m128i pack =  _mm_packus_epi16(pack1,_mm_setzero_si128());

        _mm_storeu_si32((__m128i*)&gray_map[i], pack);
    }

    // Calculating remainig pixels iteratively
    for(; i < width * height; i++) {
        if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
        else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(s[0] * (float)gray_map[i] * (float)gray_map[i] + s[1] * (float)gray_map[i] + s[2]);
    }

}


//----------------------------------Lagrange----------------------------------

void quadratic_interpolation_lagrange(uint8_t* gray_map, size_t width, size_t height,
                                       float es, float as, float em,
                                       float am, float ew, float aw) {
    float x[] = {es, em, ew};
    float y[] = {as, am, aw};
    float lagrangian[3], result;

    for(size_t i = 0; i < width * height; i++) { 
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else  { 
            // Compute lagrangian
            result = 0;
            for(size_t j = 0; j < sizeof (lagrangian) / sizeof (*lagrangian); j++) {
                lagrangian[j] = 1;
                for(size_t k = 0; k < sizeof (x) / sizeof (*x); k++) {
                    if(j == k) continue;
                    lagrangian[j] *= ((gray_map[i] - x[k]) / (x[j] - x[k]));
                }
                lagrangian[j] *= y[j];
                result += lagrangian[j]; 
            }
            if (result >= aw) gray_map[i] = aw; 
            else if (result <= as) gray_map[i] = as; 
            else gray_map[i] = result; 
        }
    }
}

void quadratic_interpolation_blagrange(uint8_t* gray_map, size_t width, size_t height,
                                       float es, float as, float em,
                                       float am, float ew, float aw) {
    // For code readability and convenience
    float x[] = {es, em, ew};
    float y[] = {as, am, aw};
    float bary_weights[3], lx, product;
  

    // Compute barycentric weights
    for (size_t i = 0; i < sizeof (bary_weights) / sizeof (*bary_weights); i++) {
        product = 1;
        for (size_t j = 0; j < sizeof (x) / sizeof (*x); j++) {
            if (i == j) continue;
                product *= (x[i] - x[j]);
        }
        bary_weights[i] = 1 / product;
    }

    //  Modified lagrange interpolation
    for (size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;       
        else if (gray_map[i] == em) gray_map[i] = am; 
        else
        {   // Compute l(x)
            lx = 1.0; 
            double sum = 0.0; 
            for (size_t j = 0; j < sizeof (x) / sizeof (*x); j++) {
                lx *= ((double)gray_map[i] - x[j]);  
                sum += ((bary_weights[j] * y[j]) / ((double) gray_map[i] - x[j]));
            }
            sum *= lx;
            if (sum <= as) gray_map[i] = as;
            else if (sum >= aw) gray_map[i]= aw;
            else gray_map[i] = sum;
        }
    }
}

//--------------------------------------Newton------------------------------------------

void quadratic_interpolation_newton(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw) {

    // For code readability and convenience
    float x[] = {es, em, ew};
    float diff_table[][3] = {{as}, {am}, {aw}};

    // Compute Newton coefficients
    for (size_t i = 1; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (size_t j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
            diff_table[j][i]  = (diff_table[j + 1][i -1] - diff_table[j][i - 1]) / (x[i +j] - x[j]);
        }
    }

    // Newton interpolation
    for(size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else  { 
                float result = (diff_table[0][0] + diff_table[0][1] * (gray_map[i] - x[0])
                    + diff_table[0][2] * (gray_map[i] - x[0]) * (gray_map[i] - x[1])); 
                if (result >= aw) gray_map[i] = aw; 
                else if (result <= as) gray_map[i] = as; 
                else gray_map[i] = result; 
        }           
    }
}

void quadratic_interpolation_newton_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw) {
    
    // Just for code readability and convenience
    float x[] = {es, em, ew};
    float diff_table[][3] = {{as}, {am}, {aw}};

    // Compute Newton coefficients
    for (size_t i = 1; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (size_t j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
            diff_table[j][i]  = (diff_table[j + 1][i -1] - diff_table[j][i - 1]) / (x[i +j] - x[j]);
        }
    }
    //Initialising LUT
    short lut[256];
    for (int i = 0; i < 256; ++i) lut[i] = -1;

    // Newton interpolation
    for(size_t i = 0; i < width * height; i++) {
        if (lut[gray_map[i]]!= -1) gray_map[i]=lut[gray_map[i]];
        else {
            uint8_t prev =gray_map[i];
            if (gray_map[i] <= es) gray_map[i] = as;
            else if (gray_map[i] >= ew) gray_map[i] = aw;
            else if (gray_map[i] == em) gray_map[i] = am;
            else  { 
                float result = (diff_table[0][0] + diff_table[0][1] * (gray_map[i] - x[0])
                    + diff_table[0][2] * (gray_map[i] - x[0]) * (gray_map[i] - x[1])); 
                if (result >= aw) gray_map[i] = aw; 
                else if (result <= as) gray_map[i] = as; 
                else gray_map[i] = result; 
            }           
            lut[prev]=gray_map[i];
        }
    }
}

//TODO
void quadratic_interpolation_newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                        float es, float as, float em,
                                        float am, float ew, float aw ,int outbound) {

    // Just for code readability and convenience
    float x[] = {es, em, ew};
    float diff_table[][3] = {{as}, {am}, {aw}};

    // Compute Newton coeffs
    for (size_t i = 1; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (size_t j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
            diff_table[j][i]  = (diff_table[j + 1][i -1] - diff_table[j][i - 1]) / (x[i +j] - x[j]);
        }
    }

    float s1 = diff_table[0][2];
    float s2 = diff_table[0][1] - (diff_table[0][2] * (x[0] + x[1]));
    float s3 = diff_table[0][0] - (diff_table[0][1] * x[0]) + (diff_table[0][2] * x[0] * x[1]);

    uint8_t intersect_aw = ew;
    uint8_t intersect_as = es;
    if((s1 < 0) && outbound) {
        intersect_aw = (uint8_t) ((- s2 / (2*s1)) - sqrt( (((float)aw - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }
    if((s1 > 0) && outbound) {
        intersect_as = (uint8_t) ((- s2 / (2*s1)) + sqrt( (((float)as - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }

    __m128 x0Vec =_mm_set_ps1(x[0]);
    __m128 x1Vec =_mm_set_ps1(x[1]);
    __m128 c0Vec =_mm_set_ps1(diff_table[0][0]);
    __m128 c1Vec =_mm_set_ps1(diff_table[0][1]);
    __m128 c2Vec =_mm_set_ps1(diff_table[0][2]);
    __m128 esVec = _mm_set_ps1(es);
    __m128 asVec = _mm_set_ps1(as);
    __m128 ewVec = _mm_set_ps1(ew);
    __m128 awVec = _mm_set_ps1(aw);
    __m128 emVec = _mm_set_ps1(em);
    __m128 amVec = _mm_set_ps1(am);

    __m128 inter_aw =_mm_set_ps1(intersect_aw);
    __m128 inter_as =_mm_set_ps1(intersect_as);

    __m128 clampvec;
     if((s1 < 0) && outbound) {
            clampvec = awVec;
        }
        if((s1 > 0) && outbound) {
            clampvec = asVec;
        }
    size_t i;
    size_t size = width * height;
    for (i = 0; i < size - (size % 4); i += 4){
        __m128 grayMapVec= _mm_set_ps((float)gray_map[i+3],(float)gray_map[i+2],
                                      (float)gray_map[i+1],(float)gray_map[i]);
        __m128 condition1 = _mm_cmple_ps(grayMapVec, esVec);
        __m128 condition2 = _mm_cmpge_ps(grayMapVec, ewVec);
        __m128 condition3 = _mm_cmpeq_ps(grayMapVec, emVec);

        __m128 clampmask;
        if((s1 < 0) && outbound) {
            clampmask = _mm_cmpgt_ps(grayMapVec, inter_aw);
        }
        if((s1 > 0) && outbound) {
            clampmask = _mm_cmple_ps(grayMapVec, inter_as);  
        }
        __m128 potentialResult = _mm_add_ps(_mm_mul_ps(c1Vec, _mm_sub_ps(grayMapVec, x0Vec)),
                                            _mm_add_ps(_mm_mul_ps(c2Vec, _mm_mul_ps(_mm_sub_ps(grayMapVec, x1Vec), _mm_sub_ps(grayMapVec, x0Vec))), c0Vec));
        __m128 result = _mm_blendv_ps( _mm_blendv_ps(potentialResult,awVec,condition2),asVec,condition1);
        result = _mm_blendv_ps(result,amVec,condition3);

        if (outbound)  result = _mm_blendv_ps(result,clampvec,clampmask);

        __m128i pack1 = _mm_packus_epi32(_mm_cvttps_epi32(result),_mm_setzero_si128());
        __m128i pack =  _mm_packus_epi16(pack1,_mm_setzero_si128());

        _mm_storeu_si32((__m128i*)&gray_map[i], pack);
    }
    
   // Calculating remainig pixels iteratively
    for(; i < width * height; i++){
        if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
        else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(diff_table[0][0] + (diff_table[0][1] * ((float)gray_map[i] - x[0]))
                + (diff_table[0][2] * ((float)gray_map[i] - x[0]) * ((float)gray_map[i] - x[1])));
    }
}
