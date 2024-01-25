
#include "test_quad_inter.h"

static int FAIL = 0; 
static int SUCCESS = 0; 

static uint8_t ARR[] = {
    0
};
static uint8_t EXP_ARR[] = { 
    0
}; 


static size_t width = ARR_SIZE / 2; 
static size_t height = ARR_SIZE / 2;

void __ASSERT__EQUAL__ARRAY(uint8_t*  expected, uint8_t* actual, size_t len, char* message) { 
    for(size_t i = 0; i < len; i++) {
        if(expected[i]!= actual[i]) {
            FAIL++;
            printf("[ FAIL ] %s\n", message);
            return; 
        }
    }
    SUCCESS++;
    printf("[ PASS ] %s\n", message);
}


void TEST_BLAGRANGE_STANDARD(void) {

    quadratic_interpolation_BLagrange(ARR, width, height, ES, AS, (ES + EW) / 2, (AS + EW) /2, EW, AW);  
    
    __ASSERT__EQUAL__ARRAY(ARR, EXP_ARR, ARR_SIZE, "TEST_BLAGRANGE_STANDARD");
} 


void TEST_BLAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST(void) { 
    
    quadratic_interpolation_BLagrange(ARR, width, height, ES, AS, (ES - EW) / 2, (AS - EW) /2, EW, AW);  
    
    __ASSERT__EQUAL__ARRAY(ARR, EXP_ARR, ARR_SIZE, "TEST_BLAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST");
}

void TEST_BLAGRANGE_HIGH_BRIGHTNESS_LOW_CONTRAST(void) { 

    quadratic_interpolation_BLagrange(ARR, width, height, ES, AS, (ES - EW) / 2, (AS - EW) /2, EW, AW);  
    
    __ASSERT__EQUAL__ARRAY(ARR, EXP_ARR, ARR_SIZE, "TEST_BLAGRANGE_HIGH_BRIGHTNESS_LOW_CONTRAST");
}

void TEST_BLAGRANGE_LOW_BRIGHTNESS_HIGHT_CONTRAST(void) { 

    quadratic_interpolation_BLagrange(ARR, width, height, ES, AS, (ES - EW) / 2, (AS - EW) /2, EW, AW);  
    
    __ASSERT__EQUAL__ARRAY(ARR, EXP_ARR, ARR_SIZE, "TEST_BLAGRANGE_LOW_BRIGHTNESS_HIGH_CONTRAST");
}

void TEST_ALL_BLAGRANGE(void) {

    TEST_BLAGRANGE_STANDARD();
    TEST_BLAGRANGE_NON_LINEAR_BRIGHTNESS_CONTRAST();
    TEST_BLAGRANGE_HIGH_BRIGHTNESS_LOW_CONTRAST();
    TEST_BLAGRANGE_LOW_BRIGHTNESS_HIGHT_CONTRAST(); 
    
}







void TEST_ALL_INTERPOLATION(void) {
   
   TEST_ALL_BLAGRANGE();

}


