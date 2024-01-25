//
// Created by Adem Trabelsi on 21.12.23.
//
#include "quadratic_interpolation.h"


//--------------------------Gaussian-----------------------------------------

void gaussian_elimination(float* x,  float es, float as,
                       float em, float am, float ew, float aw ) {

    size_t n = 3;
    float matrix[3][3] = {
        {1, ew, ew*ew},
        {1, em, em*em},
        {1, es, es*es}
    };
    float b[] = {aw, am, as};

    for (size_t j = 0; j < n; j++) { 
        for (size_t i = j+1; i < n; i++) {
            float factor = matrix[i][j] /  matrix[j][j];
            for (size_t k = j ; k < n ; k++)
                matrix[i][k] = matrix[i][k] - (factor * matrix[j][k]);
            b[i]=b[i] -  factor * b[j];   
        }
    }

     for (int i = 2 ; i >= 0 ; i--) {
        float eq = b[i];
        for (int j = 2 ; j > i ; j--)
            eq -= x[j] * matrix[i][j];
        
        x[i] = eq / matrix[i][i];
    }
}

void quadratic_interpolation_Gaussian(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw, int outbound) {

    // Solving LS using Gaussian Elimination
    float s[] = {0, 0, 0};
    gaussian_elimination(s, es, as, em, am, ew, aw);

    uint8_t intersect_aw = ew;
    uint8_t intersect_as = es;
    if((s[0] < 0) && outbound) {
        intersect_aw = (uint8_t) ((- s[1] / (2*s[0])) - sqrt( ((aw - s[2]) /s[0]) + (s[1] * s[1] / (4 * s[0] * s[0]))));
    }
    if((s[0] > 0) && outbound) {
        intersect_as = (uint8_t) ((- s[1] / (2*s[0])) + sqrt( ((as - s[2]) / s[0]) + (s[1] * s[1] / (4 * s[0] * s[0]))));
    }

    for (size_t i = 0; i< width*height; i++) {
        if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
        else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(s[0] * (float)gray_map[i] * (float)gray_map[i] + s[1] * (float)gray_map[i] + s[2]);    }
}

void quadratic_interpolation_Gaussian_LUT(uint8_t* gray_map, size_t width, size_t height,
                                float es, float as, float em,
                                float am, float ew, float aw, int outbound) {

    // Solving LS using Gaussian Elimination
    float s[] = {0, 0, 0};
    gaussian_elimination(s, es, as, em, am, ew, aw);

    uint8_t intersect_aw = ew;
    uint8_t intersect_as = es;
    if((s[0] < 0) && outbound) {
        intersect_aw = (uint8_t) ((- s[1] / (2*s[0])) - sqrt( ((aw - s[2]) /s[0]) + (s[1] * s[1] / (4 * s[0] * s[0]))));
    }
    if((s[0] > 0) && outbound) {
        intersect_as = (uint8_t) ((- s[1] / (2*s[0])) + sqrt( ((as - s[2]) / s[0]) + (s[1] * s[1] / (4 * s[0] * s[0]))));
    }

    //Initialising LUT
    short lut[256];
    for (int i = 0; i < 256; ++i) lut[i] = -1;

    // Calculating interpolation values
    for(size_t i = 0; i < width*height; i++) {
        if (lut[gray_map[i]]!= -1) gray_map[i]=lut[gray_map[i]];
        else {
            uint8_t prev =gray_map[i];
            if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
            else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
            else if (gray_map[i] == em)  gray_map[i] = am;
            else gray_map[i] = (uint8_t)(s[0] * (float)gray_map[i] * (float)gray_map[i] + s[1] * (float)gray_map[i] + s[2]);
            lut[prev]=gray_map[i];
        }
    }
}


void quadratic_interpolation_Gaussian_SIMD(uint8_t* gray_map, size_t width, size_t height,
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
    gaussian_elimination(s, es, as, em, am, ew, aw);

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

//----------------------------------Barycentric_Lagrange----------------------------------
//TODO Fix BLagrange
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
        else if (gray_map[i] == em) gray_map[i] = am; 
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

void quadratic_interpolation_BLagrange_LUT(uint8_t* gray_map, size_t width, size_t height,
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
    //Intialising LUT
    short lut[256];
    for (int i = 0; i < 256; ++i) lut[i] = -1;

    // Barycentric lagrange interpolation
    for (size_t i = 0; i < width * height; i++) {
        if (lut[gray_map[i]]!= -1) gray_map[i]=lut[gray_map[i]];
        else {
            uint8_t prev =gray_map[i];
            if (gray_map[i] <= es) gray_map[i] = as;
            else if (gray_map[i] >= ew) gray_map[i] = aw;
            else if (gray_map[i] == em) gray_map[i] = am;
            else {
                numerator = denomirator = 0;
                for (size_t j = 0; j < sizeof (bary_weights) / sizeof (*bary_weights); j++) {
                    numerator += (bary_weights[j] / (float) (gray_map[i] - x[j])) * (float) y[j];
                    denomirator += (bary_weights[j]) / (float) (gray_map[i] - x[j]);
                }
                gray_map[i] = (uint8_t) (numerator / denomirator);
            }
            lut[prev]=gray_map[i];
        }
    }
}

//--------------------------------------Newton------------------------------------------
void quadratic_interpolation_Newton(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw, int outbound) {

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
        intersect_aw = (uint8_t) ((- s2 / (2*s1)) - sqrt( ((aw - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }
    if((s1 > 0) && outbound) {
        intersect_as = (uint8_t) ((- s2 / (2*s1)) + sqrt( ((as - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }

    // Newton interpolation
    for(size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
        else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(diff_table[0][0] + diff_table[0][1] * ((float)gray_map[i] - x[0])
                + diff_table[0][2] * ((float)gray_map[i] - x[0]) * ((float)gray_map[i] - x[1]));
    }
}

void quadratic_interpolation_Newton_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    float es, float as, float em,
                                    float am, float ew, float aw, int outbound) {
    
    // Just for code readability and convenience
    float x[] = {es, em, ew};
    float diff_table[][3] = {{as}, {am}, {aw}};

    // Compute Newton coeffs
    for (size_t i = 1; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (size_t j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
            diff_table[j][i]  = (diff_table[j + 1][i -1] - diff_table[j][i - 1]) / (x[i +j] - x[j]);
        }
    }
    //Intialising LUT
    short lut[256];
    for (int i = 0; i < 256; ++i) lut[i] = -1;

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

    // Newton interpolation
    for(size_t i = 0; i < width * height; i++) {
        if (lut[gray_map[i]]!= -1) gray_map[i]=lut[gray_map[i]];
        else {
            uint8_t prev =gray_map[i];
            if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
            else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
            else if (gray_map[i] == em) gray_map[i] = am;
            else gray_map[i] = (uint8_t)(diff_table[0][0] + diff_table[0][1] * ((float)gray_map[i] - x[0])
                + diff_table[0][2] * ((float)gray_map[i] - x[0]) * ((float)gray_map[i] - x[1]));
             lut[prev]=gray_map[i];
        }
    }
}

void quadratic_interpolation_Newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                        float es, float as, float em,
                                        float am, float ew, float aw,int outbound) {

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
    //! size = width * height
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


int is_out_of_bound (float es, float as, float em, float am, float ew, float aw) {

    // upper bound
    float a_upper = (as - aw) / ((es - ew) * (es - ew));
    float b_upper = -2 * a_upper * ew;
    float c_upper = aw + (a_upper * ew * ew);
    float em_max = (a_upper * em * em) + (b_upper * em) + c_upper;
    if (em_max < am ) return 1;

    // lower bound
    float a_lower = - a_upper;
    float b_lower = -2 * a_lower * es;
    float c_lower = as + (a_lower * es * es);
    float em_min = (a_lower * em * em) + (b_lower * em) + c_lower;
    if (em_min > am ) return 1;

    return 0;
}
