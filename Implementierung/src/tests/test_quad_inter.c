
#include "test_quad_inter.h"

static uint8_t ARR[ARR_SIZE];
static uint8_t EXP_ARR[ARR_SIZE]; 

//-------------------------------------------Gray_Scale---------------------------------------------
//! No roundl
//! map not divisible by 3, OUT in read_img
void TEST_ACCURACY_GRAYSCALE_DEFAULT(void) { 
    uint8_t given[21] = 
        {0, 8, 17, 19, 23, 26, 32, 33, 41, 72, 84, 91, 110, 135, 137, 172, 199, 203, 210, 213, 244};
    uint8_t expected[7] = 
        {6, 22, 33, 81, 129, 193, 214};
    uint8_t actual[7] = {0};     

    img_to_gray_scale(actual, given, 21, 1, A, B, C);    
    
    __ASSERT__EQUAL__ARRAY(expected, actual, 7, "TEST_ACCURACY_GRAYSCALE_DEFAULT"); 
}

void TEST_ACCURACY_GRAYSCALE_TINY_INPUT(void) {
    uint8_t given[21] = 
        {0, 8, 17, 19, 23, 26, 32, 33, 41, 72, 84, 91, 110, 135, 137, 172, 199, 203, 210, 213, 244};
    uint8_t expected[7] = 
        {0, 19, 32, 72, 110, 172, 210};
    uint8_t actual[7] = {0}; 
    
    img_to_gray_scale(actual, given, 21, 1, 1.0E-3, 1.0E-5, 1.0E-7);    
    
    __ASSERT__EQUAL__ARRAY(expected, actual, 7, "TEST_ACCURACY_GRAYSCALE_TINY_INPUT"); 
}

/*
void TEST_GRAYSCALE_SIMD_DEFAULT(void) {
    srand(time(NULL));
    for (int i = 0; i < ARR_SIZE; i++) { 
        ARR[i] = rand(); 
    }

    uint8_t expected[ARR_SIZE / 3]; 
    img_to_gray_scale(expected, ARR, ARR_SIZE, 1, A, B, C); 

    uint8_t actual[ARR_SIZE / 3]; 
    img_to_gray_scale_SIMD(actual, ARR, ARR_SIZE, 1, A, B, C); 

    __ASSERT__EQUAL__ARRAY(expected, actual, ARR_SIZE / 3, "TEST_GRAYSCALE_SIMD_DEFAULT");
}
*/ 
//-------------------------------------Linear_Interpolation-----------------------------------------

void TEST_ACCURACY_LINEAR_DEFAULT(void) { 

}

void TEST_ACCURACY_LINEAR_NON_LINEAR_BRIGHTNESS_CONTRAST(void) { 

}

void TEST_LINEAR_SIMD_DEFAULT(void) {

}

void TEST_LINEAR_SIMD_NON_LINEAR_BRIGHTNESS_CONTRAST(void) { 

}

//------------------------------------Quadratic_Interpolation---------------------------------------

// Initialise expected with Newton
void init_newton(float es, float as, float em, float am, float ew, float aw) { 
    srand(time(NULL)); 
    for(int i = 0; i < ARR_SIZE; i++) {
        EXP_ARR[i] = ARR[i] = rand();
    }   
    quadratic_interpolation_newton(EXP_ARR, ARR_SIZE, 1, es, as, em, am, ew, aw);
}

// Initialise expected with Gaussian
void init_gaussian(float es, float as, float em, float am, float ew, float aw) { 
    srand(time(NULL)); 
    for(int i = 0; i < ARR_SIZE; i++) {
        EXP_ARR[i] = ARR[i] = rand();
    }   
    quadratic_interpolation_gaussian(EXP_ARR, ARR_SIZE, 1, es, as, em, am, ew, aw);
}

void TEST_ACCURACY_GAUSSIAN(void) {
    uint8_t es = 0; 
    uint8_t as = 27;
    uint8_t em = 1;
    uint8_t am = 30;
    uint8_t ew = 150;
    uint8_t aw = 200;

    // Solve linear system
    float matrix[][4] = {
        {es * es, es, 1, as},
        {em * em, em, 1, am},
        {ew * ew, ew, 1, aw}
    };
    row_reduced_echelon_form(3, 4, matrix); 

    assert__((matrix[0][3] - (-0.012394f) < ROUNDING && matrix[1][3] - 3.012396f < ROUNDING
                && matrix[2][3] - 27.f < ROUNDING)) {
        printf("[ FAIL ] TEST_ACCURACY_GAUSSIAN ON SOLVING LINEAR SYSTEM. ALL INTERPOLATION TESTS SHOULD FAIL !!\n"); 
    }; 

    uint8_t actual[20] = 
        {0, 10, 15, 19, 23, 28, 32, 39, 40, 71, 84, 91, 111, 125, 137, 172, 190, 199, 210, 213};
    
    uint8_t expected[20] = 
        {27, 55, 69, 79, 89, 101, 110, 125, 127, 178, 192, 198, 200, 200, 200, 200, 200, 200, 200, 200};
    
    quadratic_interpolation_gaussian(actual, 10, 2, es, as, em, am, ew, aw);
    
    for (int i = 0; i < 20; i++) {
        assert__((actual[i] == expected[i])) {
            printf("[ FAIL ] TEST_ACCURACY_GAUSSIAN. ALL INTERPOLATION TESTS SHOULD FAIL!!\n"); 
        };
    }
    printf("[ PASS ] TEST_ACCURACY_GAUSSIAN.\n");
}

void TEST_LAGRANGE_DEFAULT(void) {
    init_gaussian(ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW);

    quadratic_interpolation_lagrange(ARR, ARR_SIZE, 1, ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW); 
    
    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_LAGRANGE_DEFAULT");
} 

void TEST_BLAGRANGE_DEFAULT(void) {
    init_gaussian(ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW);

    quadratic_interpolation_blagrange(ARR, ARR_SIZE, 1, ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW); 
 
    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_BLAGRANGE_DEFAULT");
} 

void TEST_NEWTON_DEFAULT(void) {
    init_gaussian(ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW);

    quadratic_interpolation_newton(ARR, ARR_SIZE, 1, ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW); 
    
    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_NEWTON_DEFAULT");
} 
void TEST_LAGRANGE_OVERFLOW(void) { 
    init_gaussian(0, 27, 1, 30, 150, 200);
    
    quadratic_interpolation_lagrange(ARR, ARR_SIZE, 1, 0, 27, 1, 30, 150, 200);
    
    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_LAGRANGE_OVERFLOW");
}

void TEST_BLAGRANGE_OVERFLOW(void) {
    init_gaussian(0, 27, 1, 30, 150, 200);

    quadratic_interpolation_blagrange(ARR, ARR_SIZE, 1, 0, 27, 1, 30, 150, 200);

    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_BLAGRANGE_OVERFLOW");
}

void TEST_NEWTON_OVERFLOW(void) {
    init_gaussian(0, 27, 1, 30, 150, 200);

    quadratic_interpolation_newton(ARR, ARR_SIZE, 1, 0, 27, 1, 30, 150, 200);

    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_NEWTON_OVERFLOW");
}

void TEST_LAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST(void) { 
    init_gaussian(0, 10, 100, 180, 200, 250); 
   
    quadratic_interpolation_lagrange(ARR, ARR_SIZE, 1, 0, 10, 100, 180, 200, 250);
   
    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_LAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST");

}

void TEST_BLAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST(void) {
    init_gaussian(0, 10, 100, 180, 200, 250); 

    quadratic_interpolation_blagrange(ARR, ARR_SIZE, 1, 0, 10, 100, 180, 200, 250);

    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_BLAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST");
}

void TEST_NEWTON_NON_LINEAR_BRIGHTNESS_CONTRAST(void) {
    init_gaussian(0, 10, 100, 180, 200, 250); 

    quadratic_interpolation_newton(ARR, ARR_SIZE, 1, 0, 10, 100, 180, 200, 250);

    __ASSERT__SIMILAR__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_NEWTON_NON_LINEAR_BRIGHTNESS_CONTRAST");
}

//TODO
void TEST_GAUSSIAN_SIMD_DEFAULT(void) {
    init_gaussian(ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW);

    quadratic_interpolation_gaussian_SIMD(ARR, ARR_SIZE, 1, ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW, 1); 
    
    __ASSERT__EQUAL__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_GAUSSIAN_SIMD_DEFAULT");
}

void TEST_NEWTON_SIMD_DEFAULT(void) { 
    init_newton(ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW);

    quadratic_interpolation_newton_SIMD(ARR, ARR_SIZE, 1, ES, AS, (ES + AW) / 2, (AS + AW) / 2, EW, AW, 1); 
    
    __ASSERT__EQUAL__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_NEWTON_SIMD_DEFAULT");
}

void TEST_GAUSSIAN_SIMD_OVERFLOW(void) {
    init_gaussian(0, 27, 1, 30, 150, 200);
    
    quadratic_interpolation_gaussian_SIMD(ARR, ARR_SIZE, 1, 0, 27, 1, 30, 150, 200, 1);
    
    __ASSERT__EQUAL__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_GAUSSIAN_SIMD_OVERFLOW");
}

void TEST_NEWTON_SIMD_OVERFLOW(void) {
    init_newton(0, 27, 1, 30, 150, 200);
    
    quadratic_interpolation_newton_SIMD(ARR, ARR_SIZE, 1, 0, 27, 1, 30, 150, 200, 1);
    
    __ASSERT__EQUAL__ARRAY(EXP_ARR, ARR, ARR_SIZE, "TEST_NEWTON_SIMD_OVERFLOW");
}


void TEST_QUAD_INTERPOLATION(void) {
    TEST_ACCURACY_GRAYSCALE_DEFAULT(); 
    TEST_ACCURACY_GRAYSCALE_TINY_INPUT();
    TEST_GRAYSCALE_SIMD_DEFAULT();
    TEST_ACCURACY_GAUSSIAN(); 
    TEST_LAGRANGE_DEFAULT();
    TEST_BLAGRANGE_DEFAULT(); 
    TEST_NEWTON_DEFAULT(); 
    TEST_LAGRANGE_OVERFLOW();
    TEST_BLAGRANGE_OVERFLOW();
    TEST_NEWTON_OVERFLOW();
    TEST_GAUSSIAN_SIMD_DEFAULT();
    TEST_NEWTON_SIMD_DEFAULT();
    TEST_GAUSSIAN_SIMD_OVERFLOW();
    TEST_NEWTON_SIMD_OVERFLOW();
    TEST_LAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST();
    TEST_BLAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST();
    TEST_NEWTON_NON_LINEAR_BRIGHTNESS_CONTRAST();
    
}


 
