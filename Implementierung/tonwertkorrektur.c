//
// Created by tade on 12/16/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>
#include "io_operations.h"
 
void levels_adjustment(
        const uint8_t* img, size_t width, size_t height,
        float a, float b, float c,
        uint8_t es, uint8_t as, uint8_t em, uint8_t am,
        uint8_t ew, uint8_t aw,
        uint8_t* result
        );

      

int main(int argc, char* argv[]) {
    // Variables for getopt_long
    int option_index = 0;
    int option;

    // Flags
    int input_flag = 0, output_flag = 0, vflag = 0, bflag = 0, hflag = 0;

    // Arguments
    int varg = 0, barg = 1;
    char input_img_path[256];
    char output_img_path[256];
    double a, b, c , es, as, em, am, ew, aw;

    // Long Option flags
    static int cflag = 0, sflag = 0, mflag = 0, wflag =0;

    // Long option structs
   static const struct option longopts[] = {
        {"coeffs",  required_argument, &cflag, 'c'},
        {"lvlss",   required_argument, &sflag,  's'},
        {"lvlsm",   required_argument, &mflag, 'm'},
        {"lvlsw",   required_argument, &wflag, 'w'},
        {"help",    no_argument, NULL, 'h'},
        {0,0,0,0}
   };
   

    /*
     * B takes optional option argument
     */
    while((option = getopt_long(argc, argv, "-HO:V:B::ho:v:b::", longopts, &option_index)) != EOF) {
            switch(option) {
                //check double
                case 0: //Handle longopts
                    switch (longopts[option_index].val) {
                        case 'c':
                            ;double coeff[3];
                            if (test_args(coeff, optarg,3))
                                goto arg_error;
                            a = coeff[0];
                            b = coeff[1];
                            c = coeff[2];
                            break;
                        case 's':
                            ;double es_as[2];
                            printf("string \n");
                            printf("string %s\n", optarg);
                            if (test_args(es_as, optarg,2))
                               goto arg_error;
                            es = es_as[0];
                            as = es_as[1];
                            break;
                        case 'm':
                            ;double em_am[2];
                            if (test_args(em_am, optarg,2))
                                goto arg_error;
                            em = em_am[0];
                            am = em_am[1];
                            break;
                        case 'w':
                            ;double ew_aw[2];
                            if (test_args(ew_aw, optarg,2))
                                goto arg_error;
                            ew = ew_aw[0];
                            aw = ew_aw[1];
                            break;
                        case '?':
                            return EXIT_FAILURE;
                        default:
                            puts("UNREACHABLE\n");
                            break;
                    }
                    break;

                case 1: // case for non-option arg input data
                    //not final yet we need to count each passed argument
                    //such an error b 4 or v 89 must be handled correctly
                    if(input_flag)
                        goto arg_error;
                    if (!test_io(input_img_path, optarg))
                        goto io_error;
                    input_flag++;
                    break;
                case 'o':
                case 'O':
                    if (output_flag)
                        goto arg_error;
                    if (!test_io(output_img_path, optarg))
                        goto io_error;
                    output_flag++;
                    break;
                case 'V':
                case 'v':
                    if (vflag || !is_valid_digit(optarg, &varg))
                        goto arg_error;
                    vflag++;
                    break;
                case 'B':
                case 'b':
                    if (bflag)
                        goto arg_error;
                    bflag++;
                    if (!optarg)
                        break;
                    if(is_valid_digit(optarg, &barg))
                        goto arg_error;
                    break;
                case 'h':
                case 'H':
                    hflag++;
                    print_help();
                    return EXIT_SUCCESS;
                case '?':
                    goto arg_error;
            }
    }
    printf("barg= %d\n", barg);
    //Default values setting

    // cflag depends on the impl

    // read image
    size_t width, height;
    uint8_t img[0];
    int ret = read_img(input_img_path, img, &width, &height);
    if (ret >> 1) goto mem_error;
    if (ret) goto img_error;

    for(size_t i = 0; i < width * height; i++)
        printf("%"PRIu8 , img[i]);
    
    //grey levels conversion

    //write
    printf("%d\n" ,output_flag );
    if (output_flag) write_pgm(output_img_path,NULL,&width, &height, 255);
    else write_pgm(NULL,NULL,&width, &height, 255);
    return EXIT_SUCCESS;

    mem_error:
        fprintf(stderr, "mem_error\n");
        return EXIT_FAILURE;

    img_error:
        fprintf(stderr, "img_error\n");
        return EXIT_FAILURE;

    io_error:
        fprintf(stderr, "io_error\n");
        return EXIT_FAILURE;

    arg_error:
        fprintf(stderr, "arg_error\n");
        return EXIT_FAILURE;
}