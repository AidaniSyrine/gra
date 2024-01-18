//
// Created by tade on 12/16/23.
//
#define _POSIX_C_SOURCE 199309L

#include <time.h>

#include "io_utilities/img_io.h"
#include "io_utilities/arg_parser.h"
#include "io_utilities/ui_msgs.h"
#include "core_logic/adjustment.h"
#include "tests/test_similarity.h"

/*  Facilitate Cleanup */
#define return_dealloc(value) do { result = (value); goto dealloc; } while (0)

//TODO
static inline double curtime(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
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

    print_welcome();
    
    // Parse arguments
    if(arg_parser(&input_params, argc, argv))
        return_dealloc(EXIT_FAILURE);

    if (input_params.enable_help) {
        print_help();
        return_dealloc(EXIT_SUCCESS); 
    }
    if (input_params.enable_tests) {
        run_similarity_tests();
        return_dealloc(EXIT_SUCCESS);
    }
    
    // Read image
    printf("Loading image from: %s ...\n", input_params.input_img_path);
    if(read_img(&image_params, (const char *)input_params.input_img_path))
        return_dealloc(EXIT_FAILURE);

    
    // Allocate memory for the new grayscale 2D image
    printf("Formatting the given image to grayscale image with correction...\n");
    gray_map = (uint8_t *) malloc(image_params.width * image_params.height);
    if (gray_map == NULL) {
        fprintf(stderr, "Failed! Allocating memory to new grayscale image.\n"); 
        return_dealloc(EXIT_FAILURE); 
    }

    //TODO improvement 
    // Adjustments + Performance 
    double time;
    double start = curtime();
    switch (input_params.version) {
        case 0:
            for (int i = 0; i < input_params.iter_num; i++)
                levels_adjustment(image_params.pix_map, image_params.width,image_params.height,
                                    input_params.a,input_params.b, input_params.c,
                                    input_params.es, input_params.as, 
                                    input_params.em, input_params.am, 
                                    input_params.ew, input_params.aw, gray_map);
            break;
        // TODO 
    }
    time = curtime() - start;
    if (input_params.iter_num) 
        print_runtime(input_params.iter_num, time);     
    //TODO
 

    // Write Image
    printf("Writing image to output file...\n"); 
    if(write_img(input_params.output_img_path, gray_map, image_params.width, image_params.height))
        return_dealloc(EXIT_FAILURE);
 
    
    return_dealloc(EXIT_SUCCESS); 
    
    dealloc:
        dealloc_input_params(&input_params); 
        dealloc_image_params(&image_params);
        if (gray_map != NULL) {
            printf("-> Free: gray_map\n");
            free(gray_map); 
        }
        print_goodbye();
        return result; 
}
