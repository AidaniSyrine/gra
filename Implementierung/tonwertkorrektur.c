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
#include "placeholder.h"

static int cflag = 0, sflag = 0, mflag = 0, wflag = 0;
static const struct option longopts[] = {
        {"coeffs",  required_argument, &cflag, 'c'},
        {"lvlss",   required_argument, &sflag,  's'},
        {"lvlsm",   required_argument, &mflag, 'm'},
        {"lvlsw",   required_argument, &wflag, 'w'},
        {"help",    no_argument, NULL, 'h'},
        {0,0,0,0}
};

 struct ppm {
    size_t width;
    size_t height; 
    int maxpixel;
};

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

    //flags
    uint8_t vflag = 0, bflag = 0, hflag = 0, oflag = 0;

    //arguments
    int varg = 0, barg = 1;
    char* input_file = "";
    char* output_file = "";
    int a, b, c, e_s, a_s, e_m, a_m, e_w, a_w;

    // lowercase should work add uppercase and lower case to optstring?
    /*
     * B takes optional option argument
     */
    while((option = getopt_long(argc, argv, "ho:V:B::", longopts, &option_index)) != EOF) {
            switch(option) {
                case 0: //Handle longopts
                    switch (longopts[option_index].val) {
                            //still need to check values [0,255]
                        case 'c':
                            if (sscanf(optarg, "%d %d %d", &a, &b, &c ) != 3) {
                                fprintf(stderr, "Arg error and help\n");
                                return EXIT_FAILURE;
                            }
                            printf("a = %d, b = %d, c = %d\n", a, b, c);
                            break;
                        case 's':
                                if (sscanf(optarg, "%d %d", &e_s, &a_s) != 2) {
                                    fprintf(stderr, "Arg error and help\n");
                                    return EXIT_FAILURE;
                                }
                            printf("e_s= %d, a_s= %d\n", e_s, a_s);
                            break;
                        case 'm':
                                if (sscanf(optarg, "%d %d", &e_m, &a_m) != 2) {
                                    fprintf(stderr, "Arg error and help\n");
                                    return EXIT_FAILURE;
                                }
                            printf("e_m = %d, a_m = %d\n", e_m, a_m);
                            break;
                        case 'w':
                                if (sscanf(optarg, "%d %d", &e_w, &a_w) != 2) {
                                    fprintf(stderr, "Arg error and help\n");
                                    return EXIT_FAILURE;
                                }
                            printf("e_w = %d, a_w = %d\n", e_w, a_w);
                            break;
                    }
                    break;
                // case 1: // case for non-option arg input data
                case 'h':
                    hflag++;
                    print_help();
                    break;
                case 'o':
                    if(optarg != NULL) {
                        oflag++;
                        if (access(optarg, F_OK) != -1)
                            output_file = strncpy(output_file, optarg, strlen(optarg));
                        else {
                            fprintf(stderr, "none existing output file | Access permission denied | format?\n");
                            return EXIT_FAILURE;
                        }
                    }
                    break;
                case 'V':
                    if(optarg != NULL) {
                    vflag++;
                    //if optarg is not a valid integral number it returs 0
                    varg = atoi(optarg);
                    printf("varg= %d\n", varg);
                    }
                    else {
                        fprintf(stderr, "Argument missing\n");
                        return EXIT_FAILURE;
                    }
                    break;
                case 'B':
                    if(optarg != NULL) {
                        bflag++;
                        barg = atoi(optarg);
                        printf("barg= %d\n", barg);
                    }
                    else {
                        printf("ARG NOT specified -> Default = 1 :)\n");
                    }
                    break;
                case ':': // missing arg
                    fprintf(stderr, "Option: Required argument. See 'tonwertkorrektur --help'.\n");
                    return EXIT_FAILURE;
                    break;
                case '?': //unkonown option  ;:_M:
                    fprintf(stderr, "Unknown or ambiguous option. See 'tonwertkorrektur --help'.\n");
                    return EXIT_FAILURE;
                    break;
                
            }
    }

    if(!hflag){
        if(optind >= argc) {
            fprintf(stderr, "Positional Argument: 'input_file' is required!\n");
            return EXIT_FAILURE;
        }
        if(optind +1 != argc) {
            fprintf(stderr, "Positional Argument: only one 'input_file' is required!\n");
            return EXIT_FAILURE;
        }
        printf("Input_File= %s\n", argv[optind]);
        // check if the file exists
        if (access(argv[optind], F_OK) != -1){
                        input_file = strncpy(input_file, argv[optind], strlen(argv[optind]));
                        FILE  *input = fopen(input_file, "r");
                        if (input==NULL){
                                fprintf(stderr,"Error reading file\n");
                                 return EXIT_FAILURE;
                        }
                        struct ppm inputdim ;
                        unsigned char* pixels = isvalid(input, inputdim);
                        if (pixels ==  NULL){
                                fprintf(stderr,"format not P6\n");
                                fclose(input);
                                 return EXIT_FAILURE;
                        }
                        fclose(input);
                        greylevels ();

        }
                    else {
                        fprintf(stderr, "none existing input file | Access permission denied | format not p6\n");
                        return EXIT_FAILURE;
                    }
    }
    return EXIT_SUCCESS;
}
