#include "test_io.h"


static Image_params image_params = {0}; 

//-------------------------------Read--------------------------------

void TEST_READ_NO_ACCESS_RIGHTS(void) { 

    int result = read_img(&image_params, "../resources/testing/top_secret.ppm");
    
    __ASSERT__EQUAL(EXIT_FAILURE, result, "TEST_READ_NO_ACCESS_RIGHTS");
}

void TEST_READ_NON_REGULAR_FILE(void) {
    int result = read_img(&image_params, "../resources/testing"); 

    __ASSERT__EQUAL(EXIT_FAILURE, result, "TEST_READ_NON_REGULAR_FILE");    
}

void TEST_READ_NON_P6_FORMAT(void) {

    int result = read_img(&image_params, "../resources/testing/verifiedp3.ppm");

    __ASSERT__EQUAL(EXIT_FAILURE, result, "TEST_READ_NON_P6_FORMAT");
}

void TEST_READ_IGNORE_COMMENTS(void) {
    
    int result = read_img(&image_params, "../resources/testing/commentedp6.ppm");

    __ASSERT__EQUAL(EXIT_SUCCESS, result, "TEST_READ_IGNORE_COMMENTS");
}

void TEST_READ_NON_VALID_PIXEL(void) {

    int result = read_img(&image_params, "../resources/testing/non_valid_pixelp6.ppm");

    __ASSERT__EQUAL(EXIT_FAILURE, result, "TEST_READ_NON_VALID_PIXEL");
}

void TEST_READ_NON_VALID_DEPTH(void) {

    int result = read_img(&image_params, "../resources/testing/non_valid_depthp6.ppm");

    __ASSERT__EQUAL(EXIT_FAILURE, result, "TEST_READ_NON_VALID_DEPTH");
}
void test_normal(void) {

     int result = read_img(&image_params, "../resources/testing/verifiedp6.ppm");
    
     __ASSERT__EQUAL(EXIT_SUCCESS, result, "TEST_NORMAL"); 
} 

void TEST_READ_FUNCTIONALITY(void) {
    uint8_t pix_map[9] = {255, 0, 0, 0, 255, 0, 0, 0, 255};
    Image_params expected_params = {
        .width = 1,
        .height = 3,
        .color_depth = 255,
        .image_size = 20,
        .pix_map = pix_map
    };

    read_img(&image_params, "../resources/testing/verifiedp6.ppm"); 

    __ASSERT__EQUAL__IMGPARAMS(&expected_params, &image_params, "TEST_READ_FUNCTIONALITY");
}

//-------------------------------Write--------------------------------

void TEST_WRITE_NO_ACCESS_RIGHTS(void) { 

    int result = write_img("../resources/testing/top_secret.ppm", NULL, NULL);

    __ASSERT__EQUAL(EXIT_FAILURE, result, "TEST_WRITE_NO_ACCESS_RIGHTS");
}

void TEST_WRITE_NON_REGULAR_FILE(void) {

    int result = write_img("../resources/testing", NULL, NULL);

    __ASSERT__EQUAL(EXIT_FAILURE, result, "TEST_WRITE_NON_REGULAR_FILE"); 
}

void TEST_WRITE_FUNCTIONALITY() { 
    
    image_params.width = 2;
    image_params.height = 3;
    image_params.color_depth = 255;
    uint8_t gray_map[6] = {0, 150, 150, 0, 150, 150};

    write_img("../resources/testing/test_result.pgm", gray_map, &image_params); 

    struct stat statbuf; 
    stat("../resources/testing/test_result.pgm", &statbuf); 

    __ASSERT__EQUAL(17, statbuf.st_size, "TEST_WRITE_FUNCTIONALITY");
}


void TEST_ALL_IO_OPERATIONS(void) {
    TEST_READ_NO_ACCESS_RIGHTS(); 
    TEST_READ_NON_REGULAR_FILE(); 
    TEST_READ_NON_P6_FORMAT();
    TEST_READ_IGNORE_COMMENTS();
    TEST_READ_NON_VALID_PIXEL();
    TEST_READ_NON_VALID_DEPTH(); 
    TEST_READ_FUNCTIONALITY();
    TEST_WRITE_NO_ACCESS_RIGHTS();
    TEST_WRITE_NON_REGULAR_FILE();
    TEST_WRITE_FUNCTIONALITY();
} 