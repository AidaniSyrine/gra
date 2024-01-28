#include "unit.h"

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
    if(similarity > 75) {
        printf("[ PASS ] %s. -SIMILARITY = %d%%\n", message, (int)similarity);
        SUCCESS++;
    }
    else {
        printf("[ FAIL ] %s. -SIMILARITY = %d%%\n", message, (int)similarity);
        FAIL++;
    }
}

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

