#include "test_io.h"
#include <assert.h>

//tests that read() does not read non-p6 format
void test_read_nonp6(){
    Image_params image_params = {
        .image_ptr = NULL
    };
    int result = read_img(&image_params,"../resources/test_p6.pgm");
    assert(result==EXIT_FAILURE);
}


//tests that read() reads the dimensions correctly
void test_read_dimensions(){
    Image_params image_params = {
        .image_ptr = NULL
    };
    int result = read_img(&image_params,"../resources/sample_1280×853.ppm");
    assert(image_params.width==1280);
    assert(image_params.height==853);
    assert(image_params.color_depth==255);
    assert(result==EXIT_SUCCESS);
}

//tests that read() ignores comments properly
void test_ignore_comments(){
    Image_params image_params = {
        .image_ptr = NULL
    };
    int result = read_img(&image_params,"../resources/test_comments.ppm");
    assert(result==EXIT_SUCCESS);
    //ensure dimensions were read properly
    assert(image_params.width==512);
    assert(image_params.height==512);
    assert(image_params.color_depth==255);
}

//tests that read() does not read path
void test_read_path(){
    Image_params image_params = {
        .image_ptr = NULL
    };
    int result = read_img(&image_params,"../resources");
    assert(result==EXIT_FAILURE);
}

//tests that write() writes the correct image type
void test_write_format() {
    const uint8_t gray_map[255] = {0};
    Image_params image_params = {
        .image_ptr = NULL
    };
    int result = write_img("../resources/testOutput.pgm",gray_map,&image_params);
    assert(result==EXIT_SUCCESS);
    FILE *file = fopen("../resources/testOutput.pgm", "rb");
    char header[3]; 
    fread(header, 1, 2, file);
    header[2] = '\0'; 
    fclose(file);
    assert(strcmp(header, "P5") == 0);
}
