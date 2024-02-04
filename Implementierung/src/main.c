//
// Created by tade on 12/16/23.
//
#define _POSIX_C_SOURCE 199309L

#include <time.h>

#include "io_utilities/img_io.h"
#include "io_utilities/arg_parser.h"
#include "io_utilities/ui_msgs.h"
#include "core_logic/adjustment.h"
#include "tests/test_algorithmic.h"
#include "tests/test_io.h"
#include "tests/unittest.h"


/*  Facilitate Cleanup */
#define return_dealloc(value) do { result = (value); goto dealloc; } while (0)

static double curtime(void) {
    struct timespec time;
    if(clock_gettime(CLOCK_MONOTONIC, &time) != 0) {
        fprintf(stderr, "Failed! Could NOT get time.\n");
        return -1; 
    }
    return time.tv_sec + time.tv_nsec * 1e-9;
}

int main(int argc, char* argv[]) {
    // Initialisation
    int result;
    uint8_t* gray_map = NULL;
    
    Input_params input_params = {
        .a = A, .b = B, .c = C,
        .es = ES, .as = AS,
        .ew = EW, .aw = AW,
        .version = MAIN_VERSION,
        .iter_num = 0,
        .enable_help = false,
        .enable_tests = false, 
        .input_img_path = NULL, 
        .output_img_path = NULL 
    };
    
    Image_params image_params = {
        .image_ptr = NULL
    };

    void (*adjustments[4])(const uint8_t*, size_t, size_t, float,
                            float, float, uint8_t,uint8_t, uint8_t,
                            uint8_t, uint8_t, uint8_t, uint8_t*) = {
        levels_adjustment, 
        levels_adjustment_V1,
        levels_adjustment_V2,
        levels_adjustment_V3
    };                        
    
    
    print_welcome();
    // Parse arguments
    if(arg_parser(&input_params, argc, argv))
        return_dealloc(EXIT_FAILURE);

    if (input_params.enable_help) {
        print_help();
        return_dealloc(EXIT_SUCCESS); 
    }
    if (input_params.enable_tests) {
        start_testing("io operations");
        TEST_ALL_IO_OPERATIONS();
        start_testing("program logic");
        TEST_ALL_ALGORITHMS();
        end_testing();
        return_dealloc(EXIT_SUCCESS);
    }

    // Read image
    printf("Loading image from: %s.\n", input_params.input_img_path);
    if(read_img(&image_params, (const char *)input_params.input_img_path))
        return_dealloc(EXIT_FAILURE);

    // Allocate memory for the new grayscale 2D image
    gray_map = (uint8_t *) malloc(image_params.width * image_params.height);
    if (gray_map == NULL) {
        fprintf(stderr, "Failed! Allocating memory to new grayscale image.\n"); 
        return_dealloc(EXIT_FAILURE); 
    }

    printf("Formatting the given image to grayscale image with correction using "
            "version %d.\n", input_params.version); 

    // Performance Testing  + Conversion                         
    if (input_params.iter_num) {
        double start_time = curtime();
        if (start_time == -1) 
            return_dealloc(EXIT_FAILURE);
       
        printf("Performance testing started:\n");        
        for (int i = 0; i < input_params.iter_num; i++)
            adjustments[input_params.version](image_params.pix_map, image_params.width,
                                    image_params.height,input_params.a,input_params.b,
                                    input_params.c, input_params.es, input_params.as, 
                                    input_params.em, input_params.am, input_params.ew,
                                    input_params.aw, gray_map);           
        double stop_time = curtime();
        if (stop_time == -1) 
            return_dealloc(EXIT_FAILURE);
        double time = stop_time - start_time;
        printf("Calculation of %d iteration took %.9f seconds.\n", input_params.iter_num, time);
        printf("-> Average calculation took %.9f milliseconds.\n", (time * 1E4) / input_params.iter_num);
    } else {
        adjustments[input_params.version](image_params.pix_map, image_params.width,
                                    image_params.height,input_params.a,input_params.b,
                                    input_params.c, input_params.es, input_params.as, 
                                    input_params.em, input_params.am, input_params.ew,
                                    input_params.aw, gray_map);
    }
    
    // Write Image
    printf("Writing image to output file %s.\n", input_params.output_img_path); 
    if(write_img(input_params.output_img_path, gray_map, &image_params))
        return_dealloc(EXIT_FAILURE);
 
    
    return_dealloc(EXIT_SUCCESS); 
    
    dealloc:
        print_goodbye(!result + input_params.enable_help + input_params.enable_tests);
        dealloc_input_params(&input_params); 
        dealloc_image_params(&image_params);
        if (gray_map != NULL) {
            free(gray_map); 
        }
        return result; 
}
