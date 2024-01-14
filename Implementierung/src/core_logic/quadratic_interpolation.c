//
// Created by Adem Trabelsi on 21.12.23.
//
#include "quadratic_interpolation.h"


//TODO Recover the coeffs s1 s2 s3 for all 
void quadratic_interpolation_LS(uint8_t* gray_map, size_t width, size_t height,
                                uint8_t es, uint8_t as, uint8_t em,
                                uint8_t am, uint8_t ew, uint8_t aw) {

    // Solving LS using Gaussian Elimination
    float s1 = (float) (as - aw) * (es - ew) - (as - aw) * (es - em)
            / (float) (es * es - em * em) * (es - ew) - (es * es-ew * ew) * (es - em);
    float s2 = (float) (as - am) * (es - ew) - s1 * (es * es - em * em) * (es - ew)
            / (float)(es - em) * (es - ew);
    float s3 = as - s1 * es * es - s2 * es;

    for (size_t i = 0; i< width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = s1 * gray_map[i] * gray_map[i] + s2 * gray_map[i] + s3;
    }
}

void quadratic_interpolation_LS_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw) {

    // Solving LS using Gaussian Elimination
    float s1 = (float) (as - aw) * (es - ew) - (as - aw) * (es - em)
            / (float) (es * es - em * em) * (es - ew) - (es * es-ew * ew) * (es - em);
    float s2 = (float) (as - am) * (es - ew) - s1 * (es * es - em * em) * (es - ew)
            / (float)(es - em) * (es - ew);
    float s3 = as - s1 * es * es - s2 * es;

    //Initialising LUT
    short lut[256];
    for (int i = 0; i < 256; ++i) lut[i] = -1;

    // Calculating interpolation values
    for(size_t i = 0; i < width*height; i++) {
        if (lut[gray_map[i]]!= -1) gray_map[i]=lut[gray_map[i]];
        else {
            uint8_t prev =gray_map[i];
            if (gray_map[i] <= es)  gray_map[i] = as;
            else if (gray_map[i] >= ew)  gray_map[i] = aw;
            else if (gray_map[i] == em)  gray_map[i] = am;
            else {
                float tmp = s1 * gray_map[i] * gray_map[i];
                float tmp1 = s2 * gray_map[i];
                gray_map[i] = tmp + tmp1 + s3;
            }
            lut[prev]=gray_map[i];
        }
    }
}


void quadratic_interpolation_LS_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                  uint8_t es, uint8_t as, uint8_t em,
                                  uint8_t am, uint8_t ew, uint8_t aw)
{
    //setting registers with constants
    __m128 esVec = _mm_set_ps1((float)es);
    __m128 asVec = _mm_set_ps1((float)as);
    __m128 ewVec = _mm_set_ps1((float)ew);
    __m128 awVec = _mm_set_ps1((float)aw);

    // Solving LS using Gaussian Elimination
    float s1 = (float) (as - aw) * (es - ew) - (as - aw) * (es - em)
            / (float) (es * es - em * em) * (es - ew) - (es * es-ew * ew) * (es - em);
    float s2 = (float) (as - am) * (es - ew) - s1 * (es * es - em * em) * (es - ew)
            / (float)(es - em) * (es - ew);
    float s3 = as - s1 * es * es - s2 * es;

    __m128 s1Vec =_mm_set_ps1(s1);
    __m128 s2Vec =_mm_set_ps1(s2);
    __m128 s3Vec =_mm_set_ps1(s3);

    size_t size = width * height;
    size_t i;
    for (i = 0; i < size - (size % 4); i += 4){
        __m128 grayMapVec= _mm_set_ps((float)gray_map[i+3],(float)gray_map[i+2],
                                      (float)gray_map[i+1],(float)gray_map[i]);
        __m128 condition1 = _mm_cmple_ps(grayMapVec, esVec);
        __m128 condition2 = _mm_cmpge_ps(grayMapVec, ewVec);
        __m128 potentialResult = _mm_add_ps(_mm_mul_ps(s1Vec, _mm_mul_ps(grayMapVec, grayMapVec)),
                                            _mm_add_ps(_mm_mul_ps(s2Vec, grayMapVec), s3Vec));
        __m128 result = _mm_blendv_ps( _mm_blendv_ps(potentialResult,awVec,condition2),asVec,condition1);
        __m128i pack1 = _mm_packus_epi16(_mm_cvttps_epi32(result),_mm_cvttps_epi32(result));
        __m128i pack =  _mm_packus_epi16(pack1,pack1);

        _mm_storeu_si32((__m128i*)&gray_map[i], pack);
    }

    // Calculating remainig pixels iteratively
    for(; i < width * height; i++){
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else {
            float tmp = s1 * gray_map[i] * gray_map[i];
            float tmp1 = s2*gray_map[i];
            gray_map[i] = tmp+tmp1+s3;
        }
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
        else if (gray_map[i] == em) gray_map[i] = am; 
        else gray_map[i] = diff_table[0][0] + diff_table[0][1] * (gray_map[i] - x[0])
                + diff_table[0][2] * (gray_map[i] - x[0]) * (gray_map[i] - x[1]);
    }
}

void quadratic_interpolation_Newton_LUT(uint8_t* gray_map, size_t width, size_t height,
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
            else gray_map[i] = diff_table[0][0] + diff_table[0][1] * (gray_map[i] - x[0])
                + diff_table[0][2] * (gray_map[i] - x[0]) * (gray_map[i] - x[1]);
             lut[prev]=gray_map[i];
        }
    }
}

void quadratic_interpolation_Newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw) {

    // Just for code readability and convenience
    float x[] = {es, em, ew};
    float diff_table[][3] = {{as}, {am}, {aw}};

    // Compute Newton coeffs
    for (size_t i = 1; i < sizeof (diff_table) / sizeof (*diff_table); i++) {
        for (size_t j = 0; j < (sizeof(*diff_table) / sizeof(**diff_table)) - i; j++) {
            diff_table[j][i]  = (diff_table[j + 1][i -1] - diff_table[j][i - 1]) / (x[i +j] - x[j]);
        }
    }
    __m128 x0Vec = _mm_set_ps1(x[0]);
    __m128 x1Vec = _mm_set_ps1(x[1]);
    __m128 c0Vec = _mm_set_ps1(diff_table[0][0]);
    __m128 c1Vec = _mm_set_ps1(diff_table[0][1]);
    __m128 c2Vec = _mm_set_ps1(diff_table[0][2]);
    __m128 esVec = _mm_set_ps1((float)es);
    __m128 asVec = _mm_set_ps1((float)as);
    __m128 ewVec = _mm_set_ps1((float)ew);
    __m128 awVec = _mm_set_ps1((float)aw);
    __m128 emVec = _mm_set_ps1((float)em);
    __m128 amVec = _mm_set_ps1((float)am);

    size_t i;
    size_t size;
    for (i = 0, size = width * height; i < size - (size%4); i += 4){
        __m128 grayMapVec= _mm_set_ps((float)gray_map[i+3],(float)gray_map[i+2],
                                      (float)gray_map[i+1],(float)gray_map[i]);
        __m128 condition1 = _mm_cmple_ps(grayMapVec, esVec);
        __m128 condition2 = _mm_cmpge_ps(grayMapVec, ewVec);
        __m128 condition3 = _mm_cmpeq_ps(grayMapVec, emVec);

        __m128 potentialResult = _mm_add_ps(_mm_mul_ps(c1Vec, _mm_sub_ps(grayMapVec, x0Vec)),
                                            _mm_add_ps(_mm_mul_ps(c2Vec, _mm_mul_ps(_mm_sub_ps(grayMapVec, x1Vec)
                                            , _mm_sub_ps(grayMapVec, x0Vec))), c0Vec));
        __m128 result = _mm_blendv_ps( _mm_blendv_ps(potentialResult, awVec, condition2),asVec,condition1);
        result = _mm_blendv_ps(result,amVec,condition3);
        __m128i pack1 = _mm_packus_epi32(_mm_cvttps_epi32(result),_mm_setzero_si128());
        __m128i pack =  _mm_packus_epi16(pack1,_mm_setzero_si128());

        _mm_storeu_si32((__m128i*)&gray_map[i], pack);
    }

   // Calculating remainig bytes iteratively
    for(; i < width * height; i++){
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(diff_table[0][0] + (diff_table[0][1] * ((float)gray_map[i] - x[0]))
                + (diff_table[0][2] * ((float)gray_map[i] - x[0]) * ((float)gray_map[i] - x[1])));
    }
}



/*
void quadratic_interpolation_Newton_SIMD(uint8_t* gray_map, size_t width, size_t height,
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
    size_t size = width * height; 
    size_t i;
    uint8_t val [16]; 
    printf("ARRAY:  \n");
    for(int i = 0; i < size; i++)
        printf(", %hhu", gray_map[i]); 
    puts(" "); 


    for (size_t i = 0; i < size - (size % 16); i += 16) {
        // Load 16 bytes 
        __m128i gray_vector = _mm_loadu_si128((__m128i*) (gray_map + i));
        
        printf("gray_vector:  \n");
        memcpy(val, &gray_vector, sizeof(val));
        for(int j = 0; j < 16; j++)
        printf(", %hhu", val[j]); 
        puts(" "); 
        

        // mask those <=es 
        __m128i es_vector = _mm_set1_epi8(es);
        __m128i mask_es = _mm_max_epu8(gray_vector, es_vector);
        mask_es = _mm_cmpeq_epi8(es_vector, mask_es);

        //mask those ==em
        __m128i mask_em = _mm_cmpeq_epi8(gray_vector, _mm_set1_epi8(em));
        
        //mask those >=ew
        __m128i ew_vector = _mm_set1_epi8(ew);
        __m128i mask_ew = _mm_min_epu8(gray_vector, ew_vector); 
        mask_ew = _mm_cmpeq_epi8(ew_vector, mask_ew); 


        printf("mask_ew:  \n");
        memcpy(val, &mask_ew, sizeof(val));
        for(int j = 0; j < 16; j++)
        printf(", %hhu", val[j]); 
        puts(" "); 
        

        
        
        //gray_map - x[_] 
        __m128i sub_x1_vector = _mm_subs_epu8(gray_vector, _mm_set1_epi8(x[1]));
        __m128i sub_x0_vector = _mm_subs_epu8(gray_vector, _mm_set1_epi8(x[0]));


        





    }
                                    
}
int main() {
    uint8_t es = 0; 
    uint8_t as = 0;
    uint8_t em = 127.5; 
    uint8_t am = 127.5;
    uint8_t ew = 255;
    uint8_t aw = 255;

    uint8_t array[] =  {0, 1, 2, 3, 4,5,6,7,8,9,
                        10,20,30,40,50,60,70,80,90,100,
                        110,120,130,140,150,160,170,180,190,
                        200,210,220,230,240, 250};
    size_t width = 7; 
    size_t height = 5; 
    quadratic_interpolation_Newton(array, width, height, es, as, em, am, ew, aw);
    //quadratic_interpolation_Newton_SIMD(array, width, height, es, as, em, am, ew, aw); 

}
*/
  


