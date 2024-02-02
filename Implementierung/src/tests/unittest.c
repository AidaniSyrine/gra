#include "unittest.h"

static int FAIL = 0; 
static int SUCCESS = 0; 


void __ASSERT__SIMILAR__ARRAY(uint8_t* expected, uint8_t* actual, size_t len, char* message) {
    double similarity = 0; 
    for(size_t i = 0; i < len; i++) {
        if(expected[i]== actual[i]) {
           similarity++;
        }
    }
    similarity =  (similarity / len) * 100; 
    if(similarity > 76) {
        printf("[PASS] %s. -SIMILARITY = %d%%\n", message, (int)similarity);
        SUCCESS++;
    }
    else {
        printf("[FAIL] %s. -SIMILARITY = %d%%\n", message, (int)similarity);
        FAIL++;
    }
}

void __ASSERT__EQUAL__ARRAY(uint8_t*  expected, uint8_t* actual, size_t len, char* message) { 
    for(size_t i = 0; i < len; i++) {
        if(expected[i]!= actual[i]) {
            FAIL++;
            printf("[FAIL] %s\n", message);
            return; 
        }
    }
    SUCCESS++;
    printf("[PASS] %s\n", message);
}

void __ASSERT__EQUAL(int expected, int actual, char* message) {
    if (expected != actual) {
        FAIL++;
        printf("[FAIL] %s\n", message);
        return; 
    }

    SUCCESS++;
    printf("[PASS] %s\n", message);
}

void __ASSERT__EQUAL__IMGPARAMS(Image_params* expected, Image_params* actual, char* message) {
    if (expected->height != actual->height || expected->width != actual->width 
        || expected->color_depth != actual->color_depth || expected->image_size != actual->image_size) {
            FAIL++;
            printf("[FAIL] %s\n", message);
            return;
        } 
    __ASSERT__EQUAL__ARRAY(expected->pix_map, actual->pix_map, expected->width * expected->height, message);
}


void start_testing(char* message) { 
    (void)! freopen("/dev/null", "w", stderr); 
    printf("===========================================================================================\n"); 
    printf("-> Testing %s started: \n", message);
}

void end_testing() {
    printf("===========================================================================================\n"); 
    printf("[===>] Synthesis: Tested: %d | Passing: %d | Failing: %d\n", FAIL + SUCCESS, SUCCESS, FAIL);
    printf("===========================================================================================\n"); 
}
