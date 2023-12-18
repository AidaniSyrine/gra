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
#include <ctype.h>
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
    int vflag = 0, bflag = 1, hflag = 0, oflag = 0;

    // Arguments
    int varg = 0, barg = 1;
    char input_img_path[256];
    char output_img_path[256];
    int a, b, c, es, as, em, am, ew, aw;

    // Long Option flags
    static int cflag = 0, sflag = 0, mflag = 0, wflag =0;

    // Long option structs
   static const struct option longopts[] = {
        {"coeffs",  required_argument, &cflag, 'c'},
        {"lvlss",   required_argument, &sflag,  's'},
        {"lvlsm",   required_argument, &mflag, 'm'},
        {"lvlsw",   required_argument, &wflag, 'w'},
        {"help",    no_argument, 0, 'h'},
        {0,0,0,0}
   };

    /*
     * B takes optional option argument
     */
    while((option = getopt_long(argc, argv, "-HO:V:B::ho:v:b::", longopts, &option_index)) != EOF) {
            switch(option) {
                case 0: //Handle longopts
                    switch (longopts[option_index].val) {
                        case 'c':
                            int coeff[3];
                            if (!test_coeff_args(coeff, argv, optind - 1))
                                goto arg_error;
                            a = coeff[0];
                            b = coeff[1];
                            c = coeff[2];
                            break;
                        case 's':
                            int es_as[2];
                            if (!test_lvl_args(es_as, argv, optind - 1))
                                goto arg_error;
                            es = es_as[0];
                            as = es_as[1];
                            break;
                        case 'm':
                            int em_am[2];
                            if (!test_lvl_args(em_am, argv, optind - 1))
                                goto arg_error;
                            em = em_am[0];
                            am = em_am[1];
                            break;
                        case 'w':
                            int ew_aw[2];
                            if (!test_lvl_args(ew_aw, argv, optind - 1))
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
                    if(!test_io(input_img_path, optarg))
                        goto io_error;
                    break;
                case 'o':
                case 'O':
                    if(!test_io(output_img_path, optarg))
                        goto io_error;
                    oflag++;
                    break;
                case 'V':
                case 'v':
                    for (int i = 0; optarg[i] != '\0'; i++)
                        if (!isdigit(optarg[i]))
                            goto arg_error;
                    vflag++;
                    varg = atoi(optarg);
                    break;
                case 'B':
                case 'b':
                    if(optarg == NULL)
                        break;
                    for (int i = 0; optarg[i] != '\0'; i++)
                        if(!isdigit(optarg[i]))
                            goto arg_error;
                    bflag++;
                    barg = atoi(optarg);
                    break;
                case 'h':
                case 'H':
                    hflag++;
                    print_help();
                    return EXIT_SUCCESS;
                case '?':
                    goto arg_error;
                    break;
            }
    }
    //Default values setting
    es = as = (!sflag)? ES : es;


    // cflag depends on the impl
    // read image to array of pixel struct

    return EXIT_SUCCESS;
    io_error:
        fprintf(stderr, "io_error\n");
        return EXIT_FAILURE;

    arg_error:
        fprintf(stderr, "arg_error\n");
        return EXIT_FAILURE;
}
