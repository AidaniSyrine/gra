
#include "unittest.h"

#include "unittest.h"


Image_params image_params = {
        .image_ptr = NULL
};


void run_all_tests(void) { 
    printf("Testing...\n");
    printf("__________________________\n");
    printf("Starting I/O tests:\n");
    printf("\n");
    test_read_nonp6();
    printf("test read non P6 format passed.\n");
    test_read_dimensions();
    printf("test read dimensions passed.\n");
    //test_ignore_comments();
    //printf("test ignore comments passed.\n");
    test_read_path();
    printf("test read path passed.\n");
    test_write_format();
    printf("test write format passed.\n");
    printf("__________________________\n");
    printf("Starting similarity tests:\n");
    printf("\n");
    //reading the image for tests
    read_img(&image_params,"../resources/mandrill.ppm" );
    //testing grasycale SIMD
    test_grayscale_SIMD(&image_params);
    printf("test graysclae SIMD passed.\n");
    //testing linear simd
    test_linear_SIMD(&image_params);
    printf("test linear SIMD passed.\n");
    //testing quadratic simd
    read_img(&image_params,"../resources/mandrill.ppm" );
    test_quadraticLS_SIMD(&image_params);
    printf("test quadratic interpolation SIMD passed.\n");
    //testing quadratic newton SIMD
    test_newton_SIMD(&image_params) ;
    printf("test newton simd passed.\n");
    //testing quadratic variations
    test_quadratic(&image_params) ;
    printf("test quadratic variations passed.\n");
    printf("__________________________\n");
    printf("All tests passed succesfully.\n");
}

void run_pers_tests(void) { 
    TEST_ALL_INTERPOLATION();

}