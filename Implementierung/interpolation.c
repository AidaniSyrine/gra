//
// Created by Adem Trabelsi on 21.12.23.
//
#include "interpolation.h"
#include<stdio.h>
#include<math.h>

// --------------------------- Linear Interpolation --------------------------------------

void linear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw)
{
   for(size_t i = 0; i < width * height; i++) {
       if (gray_map[i] <= es) gray_map[i] = as;
       else if (gray_map[i] >= ew) gray_map[i] = aw;
       else if (gray_map[i] == em) gray_map[i] = am;
       else if (gray_map[i] < em)
           gray_map[i] = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
       else
           gray_map[i] = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
   }
}

void linear_interpolation_LUT(uint8_t* gray_map, size_t width, size_t height,
                              uint8_t es, uint8_t as, uint8_t em,
                              uint8_t am, uint8_t ew, uint8_t aw) {
    short lut[256];
    for (int i = 0; i < 256; ++i) lut[i] = -1;

    for(size_t i = 0; i < width*height; i++) {
        if (lut[gray_map[i]]!= -1) gray_map[i]=lut[gray_map[i]];
        else {
            uint8_t prev =gray_map[i];
            if (gray_map[i] <= es)  gray_map[i] = as;
            else if (gray_map[i] >= ew)  gray_map[i] = aw;
            else if (gray_map[i] == em)  gray_map[i] = am;
            else if (gray_map[i] < em)   
                gray_map[i] = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
            else 
                gray_map[i] = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
            lut[prev]=gray_map[i];
        }
    }
}

// TODO [-Woverflow]
void linear_interpolation_SIMD(uint8_t* gray_map, size_t width, size_t height,
                               uint8_t es, uint8_t as, uint8_t em,
                               uint8_t am, uint8_t ew, uint8_t aw)
{
    __m128i simd_es = _mm_set1_epi16(es);
    __m128i simd_as = _mm_set1_epi8(as);
    __m128i simd_em = _mm_set1_epi16(em);
    __m128i simd_am = _mm_set1_epi8(am);
    __m128i simd_ew = _mm_set1_epi16(ew);

    __m128i simd_factor1 = _mm_set1_epi8((am - as) / (em - es));
    __m128i simd_factor2 = _mm_set1_epi8((uint8_t)((aw - am) / (ew - em)));

    size_t size = width * height;
    size_t i =0;
    for (i = 0; i < size - (size % 16); i += 16) {
        //loading 16 bytes
        __m128i simd_gray = _mm_loadu_si128((__m128i*)(gray_map+i));

        // change bytes <es to 0
        __m128i mask_es = _mm_cmpgt_epi16(_mm_unpacklo_epi8(simd_gray, _mm_setzero_si128()), simd_es);
        mask_es = _mm_packs_epi16 (mask_es,_mm_cmpgt_epi16(_mm_unpackhi_epi8(simd_gray,
                                                                             _mm_setzero_si128()), simd_es));
        mask_es = _mm_andnot_si128(mask_es, _mm_set1_epi8((uint8_t)0xff));
        __m128i result = _mm_blendv_epi8(simd_gray, _mm_set1_epi8(0),  mask_es);

        // change bytes >ew to 0
        __m128i mask_ew = _mm_cmplt_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()), simd_ew);
        mask_ew = _mm_packs_epi16 (mask_ew,_mm_cmplt_epi16(_mm_unpackhi_epi8(result,
                                                                             _mm_setzero_si128()), simd_ew));
        mask_ew = _mm_andnot_si128(mask_ew, _mm_set1_epi8((uint8_t)0xff));

        result = _mm_blendv_epi8(result, _mm_set1_epi8(0),  mask_ew);

        // register of adequat factors
        __m128i mask_ltem = _mm_cmplt_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()), simd_em);
        mask_ltem = _mm_packs_epi16 (mask_ltem,_mm_cmplt_epi16(_mm_unpackhi_epi8(result,
                                                                                 _mm_setzero_si128()), simd_em));

        __m128i factors = _mm_and_si128(mask_ltem, simd_factor1 ) ;
        __m128i mask_getem =_mm_andnot_si128(mask_ltem, _mm_set1_epi8((uint8_t)0xff));

        factors = _mm_or_si128(factors, _mm_and_si128(mask_getem, simd_factor2 ) );

        // register of adequat constant to be added
        __m128i addconstants = _mm_and_si128(mask_ltem, simd_as ) ;
        addconstants = _mm_or_si128(addconstants, _mm_and_si128(mask_getem, simd_am ) );

        //register of adequat constant to be substructed
        __m128i subconstants = _mm_and_si128(mask_ltem, _mm_set1_epi8(es) ) ;
        subconstants = _mm_or_si128(subconstants, _mm_and_si128(mask_getem, _mm_set1_epi8(em) ) );

        // substraction
        result= _mm_sub_epi8 (result,subconstants);

        // first 8 bytes
        __m128i final_result1 = _mm_maddubs_epi16(_mm_unpacklo_epi8(result, _mm_setzero_si128()),
                                                  _mm_unpacklo_epi8(factors, _mm_setzero_si128()));
        __m128i final_result2 = _mm_maddubs_epi16(_mm_unpackhi_epi8(result, _mm_setzero_si128()),
                                                  _mm_unpackhi_epi8(factors, _mm_setzero_si128()));
        final_result1 = _mm_packus_epi16 (final_result1,final_result2);

        // add constant
        final_result1 = _mm_add_epi8(final_result1, addconstants);

        // if <es return as ; if >ew return aw ; if ==em return am
        final_result1 = _mm_blendv_epi8(final_result1, _mm_set1_epi8(aw),  mask_ew);
        final_result1 = _mm_blendv_epi8(final_result1, _mm_set1_epi8(as),  mask_es);
        final_result1 = _mm_blendv_epi8(final_result1, _mm_set1_epi8(am),
                                        _mm_cmpeq_epi8(simd_gray, _mm_set1_epi8(em)));

        // Convert back to uint8_t and store the result
        _mm_storeu_si128((__m128i_u *) (gray_map + i), final_result1);
    }

    // Complete remaining pixels iteratively
    for(; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else if (gray_map[i] <= em)
            gray_map[i] = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
        else
            gray_map[i] = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
    }
    return;
}


void bilinear_interpolation(uint8_t* gray_map, size_t width, size_t height,
                                uint8_t es, uint8_t as, uint8_t em,
                                uint8_t am, uint8_t ew, uint8_t aw) {
    for(size_t i = 0; i < width * height; i++) {
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else if (gray_map[i] < em) {
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

void bilinear_interpolation_LUT(uint8_t* gray_map, size_t width, size_t height,
                                uint8_t es, uint8_t as, uint8_t em,
                                uint8_t am, uint8_t ew, uint8_t aw) {
    //Intialising LUT
    short lut[256];
    for (int i = 0; i < 256; ++i) lut[i] = -1;

    for(size_t i = 0; i < width * height; i++) {
        if (lut[gray_map[i]]!= -1) gray_map[i]=lut[gray_map[i]];
        else {
            uint8_t prev =gray_map[i];
            if (gray_map[i] <= es) gray_map[i] = as;
            else if (gray_map[i] >= ew) gray_map[i] = aw;
            else if (gray_map[i] == em) gray_map[i] = am;
            else if (gray_map[i] < em) {
                uint8_t first_inter = as +  ((am - as) / (em - es)) * (gray_map[i] - es);
                uint8_t second_inter = as + ((aw - as) / (ew - es)) * (gray_map[i] - es);
                gray_map[i] = (first_inter + second_inter) / 2;
            } else {
                uint8_t first_inter = am + ((aw - am) / (ew - em)) * (gray_map[i] - em);
                uint8_t second_inter = as + ((aw - as) / (ew - es)) * (gray_map[i] - es);
                gray_map[i] = (first_inter + second_inter) / 2;
            }
            lut[prev]=gray_map[i];
        }

    }
}


// --------------------------- Quadratic Interpolation --------------------------------------

void quadratic_interpolation_LS(uint8_t* gray_map, size_t width, size_t height,
                                uint8_t es, uint8_t as, uint8_t em,
                                uint8_t am, uint8_t ew, uint8_t aw, int outbound) {

    float asf=as; float amf=am; float awf=aw; float esf=es; float emf=em; float ewf=ew;
    // Solving LS using Gaussian Elimination
    float s1 = (float) (((asf - amf) * (esf - ewf)) - ((asf - awf) * (esf - emf)))
            / (float)((((esf * esf) - (emf * emf))* (esf - ewf)) - (((esf * esf)- (ewf * ewf)) * (esf - emf)));
    float s2 = (float) (((asf - amf) * (esf - ewf) )- (s1 * ((esf * esf) - (emf * emf)) * (esf - ewf)))
            / (float)((esf - emf) * (esf - ewf));
    float s3 = asf - s1 * esf * esf - s2 * esf;

    printf("s1 %f s2 %f s3 %f\n ",s1,s2,s3);
    
    uint8_t intersect_aw = ew;
    uint8_t intersect_as = es;
    if((s1 < 0) && outbound) {
        intersect_aw = (uint8_t) ((- s2 / (2*s1)) - sqrt( ((awf - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }
    if((s1 > 0) && outbound) {
        intersect_as = (uint8_t) ((- s2 / (2*s1)) + sqrt( ((asf - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }

    for (size_t i = 0; i< width*height; i++) {
        if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
        else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(s1 * (float)gray_map[i] * (float)gray_map[i] + s2 * (float)gray_map[i] + s3);
    }
}

void quadratic_interpolation_LS_LUT(uint8_t* gray_map, size_t width, size_t height,
                                    uint8_t es, uint8_t as, uint8_t em,
                                    uint8_t am, uint8_t ew, uint8_t aw, int outbound) {

    float asf=as; float amf=am; float awf=aw; float esf=es; float emf=em; float ewf=ew;
    // Solving LS using Gaussian Elimination
    float s1 = (float) (((asf - amf) * (esf - ewf)) - ((asf - awf) * (esf - emf)))
            / (float)((((esf * esf) - (emf * emf))* (esf - ewf)) - (((esf * esf)- (ewf * ewf)) * (esf - emf)));
    float s2 = (float) (((asf - amf) * (esf - ewf) )- (s1 * ((esf * esf) - (emf * emf)) * (esf - ewf)))
            / (float)((esf - emf) * (esf - ewf));
    float s3 = asf - s1 * esf * esf - s2 * esf;

     uint8_t intersect_aw = ew;
    uint8_t intersect_as = es;
    if((s1 < 0) && outbound) {
        intersect_aw = (uint8_t) ((- s2 / (2*s1)) - sqrt( ((awf - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }
    if((s1 > 0) && outbound) {
        intersect_as = (uint8_t) ((- s2 / (2*s1)) + sqrt( ((asf - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }

    //Intialising LUT
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
            else gray_map[i] = (uint8_t)(s1 * (float)gray_map[i] * (float)gray_map[i] + s2 * (float)gray_map[i] + s3);
            lut[prev]=gray_map[i];
        }
    }
}


void quadratic_interpolation_LS_SIMD(uint8_t* gray_map, size_t width, size_t height,
                                  uint8_t es, uint8_t as, uint8_t em,
                                  uint8_t am, uint8_t ew, uint8_t aw, int outbound)
{
    //setting registers with constants
    __m128 esVec = _mm_set_ps1((float)es);
    __m128 asVec = _mm_set_ps1((float)as);
    __m128 ewVec = _mm_set_ps1((float)ew);
    __m128 awVec = _mm_set_ps1((float)aw);
    __m128 emVec = _mm_set_ps1((float)em);
    __m128 amVec = _mm_set_ps1((float)am);

    float asf=as; float amf=am; float awf=aw; float esf=es; float emf=em; float ewf=ew;
    // Solving LS using Gaussian Elimination
    float s1 = (float) (((asf - amf) * (esf - ewf)) - ((asf - awf) * (esf - emf)))
            / (float)((((esf * esf) - (emf * emf))* (esf - ewf)) - (((esf * esf)- (ewf * ewf)) * (esf - emf)));
    float s2 = (float) (((asf - amf) * (esf - ewf) )- (s1 * ((esf * esf) - (emf * emf)) * (esf - ewf)))
            / (float)((esf - emf) * (esf - ewf));
    float s3 = asf - s1 * esf * esf - s2 * esf;

    uint8_t intersect_aw = ew;
    uint8_t intersect_as = es;
    if((s1 < 0) && outbound) {
        intersect_aw = (uint8_t) ((- s2 / (2*s1)) - sqrt( ((awf - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }
    if((s1 > 0) && outbound) {
        intersect_as = (uint8_t) ((- s2 / (2*s1)) + sqrt( ((asf - s3) / s1) + (s2 * s2 / (4 * s1 * s1))));
    }

    __m128 s1Vec =_mm_set_ps1(s1);
    __m128 s2Vec =_mm_set_ps1(s2);
    __m128 s3Vec =_mm_set_ps1(s3);

    __m128 inter_aw =_mm_set_ps1(intersect_aw);
    __m128 inter_as =_mm_set_ps1(intersect_as);


    size_t size = width * height;
    size_t i;
    for (i = 0; i < size - (size % 4); i += 4){
        __m128 grayMapVec= _mm_set_ps((float)gray_map[i+3],(float)gray_map[i+2],
                                      (float)gray_map[i+1],(float)gray_map[i]);
        __m128 condition1 = _mm_cmple_ps(grayMapVec, esVec);
        __m128 condition2 = _mm_cmpge_ps(grayMapVec, ewVec);
        __m128 condition3 = _mm_cmpeq_ps(grayMapVec, emVec);
        __m128 clampmask, clampvec;
        if((s1 < 0) && outbound) {
            clampmask = _mm_cmpgt_ps(grayMapVec, inter_aw);
            clampvec = awVec;
        }
        if((s1 > 0) && outbound) {
            clampmask = _mm_cmple_ps(grayMapVec, inter_as);  
            clampvec = asVec;
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
    for(; i < width * height; i++){
        if (gray_map[i] <= es || gray_map[i] <= intersect_as) gray_map[i] = as;
        else if (gray_map[i] >= ew || gray_map[i] > intersect_aw ) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(s1 * (float)gray_map[i] * (float)gray_map[i] + s2 * (float)gray_map[i] + s3);

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
    float x[] = {es, em, ew};
    float diff_table[][3] = {{as}, {am}, {aw}};

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
        else gray_map[i] = (uint8_t)(diff_table[0][0] + diff_table[0][1] * ((float)gray_map[i] - x[0])
                + diff_table[0][2] * ((float)gray_map[i] - x[0]) * ((float)gray_map[i] - x[1]));
    }
}

void quadratic_interpolation_Newton_LUT(uint8_t* gray_map, size_t width, size_t height,
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
     //Intialising LUT
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
            else gray_map[i] = (uint8_t)(diff_table[0][0] + diff_table[0][1] * ((float)gray_map[i] - x[0])
                + diff_table[0][2] * ((float)gray_map[i] - x[0]) * ((float)gray_map[i] - x[1]));
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
    __m128 x0Vec =_mm_set_ps1(x[0]);
    __m128 x1Vec =_mm_set_ps1(x[1]);
    __m128 c0Vec =_mm_set_ps1(diff_table[0][0]);
    __m128 c1Vec =_mm_set_ps1(diff_table[0][1]);
    __m128 c2Vec =_mm_set_ps1(diff_table[0][2]);
    __m128 esVec = _mm_set_ps1((float)es);
    __m128 asVec = _mm_set_ps1((float)as);
    __m128 ewVec = _mm_set_ps1((float)ew);
    __m128 awVec = _mm_set_ps1((float)aw);
    __m128 emVec = _mm_set_ps1((float)em);
    __m128 amVec = _mm_set_ps1((float)am);

    size_t i;
    size_t size;
    for (i = 0; i < size - (size%4); i += 4){
        __m128 grayMapVec= _mm_set_ps((float)gray_map[i+3],(float)gray_map[i+2],
                                      (float)gray_map[i+1],(float)gray_map[i]);
        __m128 condition1 = _mm_cmple_ps(grayMapVec, esVec);
        __m128 condition2 = _mm_cmpge_ps(grayMapVec, ewVec);
        __m128 condition3 = _mm_cmpeq_ps(grayMapVec, emVec);

        __m128 potentialResult = _mm_add_ps(_mm_mul_ps(c1Vec, _mm_sub_ps(grayMapVec, x0Vec)),
                                            _mm_add_ps(_mm_mul_ps(c2Vec, _mm_mul_ps(_mm_sub_ps(grayMapVec, x1Vec), _mm_sub_ps(grayMapVec, x0Vec))), c0Vec));
        __m128 result = _mm_blendv_ps( _mm_blendv_ps(potentialResult,awVec,condition2),asVec,condition1);
        result = _mm_blendv_ps(result,amVec,condition3);
        __m128i pack1 = _mm_packus_epi32(_mm_cvttps_epi32(result),_mm_setzero_si128());
        __m128i pack =  _mm_packus_epi16(pack1,_mm_setzero_si128());

        _mm_storeu_si32((__m128i*)&gray_map[i], pack);
    }

   // Calculating remainig pixels iteratively
    for(; i < width * height; i++){
        if (gray_map[i] <= es) gray_map[i] = as;
        else if (gray_map[i] >= ew) gray_map[i] = aw;
        else if (gray_map[i] == em) gray_map[i] = am;
        else gray_map[i] = (uint8_t)(diff_table[0][0] + (diff_table[0][1] * ((float)gray_map[i] - x[0]))
                + (diff_table[0][2] * ((float)gray_map[i] - x[0]) * ((float)gray_map[i] - x[1])));
    }
}


