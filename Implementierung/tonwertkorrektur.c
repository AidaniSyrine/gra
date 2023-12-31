//
// Created by tade on 12/16/23.
//
#define _POSIX_C_SOURCE 199309L

#include <getopt.h>
#include <time.h>
#include "io_operations.h"
#include "adjustment.h"


static int cflag = 0, sflag = 0, mflag = 0, wflag =0;
static const struct option longopts[] = {
        {"coeffs",  required_argument, &cflag, 'c'},
        {"lvlss",   required_argument, &sflag,  's'},
        {"lvlsm",   required_argument, &mflag, 'm'},
        {"lvlsw",   required_argument, &wflag, 'w'},
        {"help",    no_argument, 0, 'h'},
        {0,0,0,0}
};

static inline double curtime(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

int main(int argc, char* argv[]) {
    // get-opt variables
    int option_index = 0;
    int option;

    // Flags
    int input_flag = 0, output_flag = 0, vflag = 0, bflag = 0;

    // Arguments and (Meta) data
    int version = 0, iterations = 1;
    char input_img_path[512], output_img_path[512];
    float a, b, c;
    uint8_t es, as, em, am, ew, aw;


    // Read options
    char valid_args[3];
    while((option = getopt_long(argc, argv, "-HO:V:B::ho:v:b::", longopts, &option_index)) != EOF) {
            switch(option) {
                case 0: // Handle longopts
                    switch (longopts[option_index].val) {
                        case 'c':
                            if (test_and_set_largs(valid_args,  &optarg, cflag)) goto arg_error;
                            a = ((float *) valid_args)[0]; b = ((float *) valid_args)[1]; c = ((float*) valid_args)[2];
                            break;
                        case 's':
                            if (test_and_set_largs(valid_args, &optarg, sflag)) goto arg_error;
                            es = ((uint8_t*) valid_args)[0]; as = ((uint8_t*) valid_args)[1];
                            break;
                        case 'm':
                            if (test_and_set_largs(valid_args, &optarg, mflag)) goto arg_error;
                            em = ((uint8_t*) valid_args)[0]; am = ((uint8_t*) valid_args)[1];
                            break;
                        case 'w':
                            if (test_and_set_largs(valid_args, &optarg, wflag)) goto arg_error;
                            ew = ((uint8_t*) valid_args)[0]; aw = ((uint8_t*) valid_args)[1];
                            break;
                        case '?':
                            puts("UNREACHABLE!");
                            return EXIT_FAILURE;
                    } break;
                case 1: // Non-option arg input file
                    if(input_flag) goto arg_error;
                    if (test_and_set_input(input_img_path, optarg)) goto input_error;
                    input_flag++;
                    printf("input= %s \n", input_img_path);
                    break;
                case 'o': case 'O':
                    if (output_flag) goto arg_error;
                    if (test_and_set_output(output_img_path, optarg)) goto output_error;
                    output_flag++;
                    printf("output = %s\n", output_img_path);
                    break;
                case 'V': case 'v':
                    if (vflag || test_and_set_sarg(&version, optarg)) goto arg_error;
                    // TODO: set number of versions
                    if (version < 0 || version > 6) goto arg_error;
                    vflag++;
                    printf("varg= %d\n", version);
                    break;
                case 'B': case 'b':
                    if (bflag) goto arg_error;
                    bflag++;
                    if (!optarg) {
                        printf("num of iterations is = %d\n", iterations);
                        break;
                    }
                    if (test_and_set_sarg(&iterations, optarg)) goto arg_error;
                    if (iterations <= 0) goto arg_error;
                    printf("num of iterations is = %d\n", iterations);
                    break;
                case 'h': case 'H':
                    print_help();
                    return EXIT_SUCCESS;
                case '?': default:
                    goto arg_error;
            }
    }
    // No input fle
    if (!input_flag) goto arg_error;

    // Check if the input is valid
    if (sflag && mflag && wflag)
            if (es > ew || es> em || ew < em || as > aw || as > am || aw < am) goto arg_error;

    // Setting Default value
    if (!sflag)  es = as = AS;
    if (!wflag) ew = aw = AW;
    if (!mflag) {
        em = (es + aw) / 2;
        am = (as + aw) / 2;
    }
    if(!cflag) {
        a = A; b = B; c = C;
    }

    // Read image
    size_t width, height;
    uint8_t color_depth, *pix_map = NULL;
    int ret = read_img(input_img_path, &pix_map, &width, &height, &color_depth);
    if (ret == EXIT_MEM_FAILURE) goto mem_error;
    if (ret == EXIT_FAILURE) goto img_error;

    // Allocate memory for the new grayscale 2D image
    uint8_t* gray_map = (uint8_t *) malloc(width * height);
    if (!gray_map) goto mem_error;


    // Adjustments

    double time;
    double start = curtime();

    switch (version) {
        case 0:
            for (int i = 0; i < iterations; i++)
                levels_adjustment(pix_map, width, height, a, b, c, es, as, em, am, ew, aw, gray_map);
            break;
        case 1:
            for (int i = 0; i < iterations; i++)
                levels_adjustment_V1(pix_map, width, height, a, b, c, es, as, em, am, ew, aw, gray_map);
            break;
        case 2:
             for (int i = 0; i < iterations; i++)
                levels_adjustment_V2(pix_map, width, height, a, b, c, es, as, em, am, ew, aw, gray_map);
            break;
        case 3:
            for (int i = 0; i < iterations; i++)
                levels_adjustment_V3(pix_map, width, height, a, b, c, es, as, em, am, ew, aw, gray_map);
            break;
        case 4:
            for (int i = 0; i < iterations; i++)
                levels_adjustment_V4(pix_map, width, height, a, b, c, es, as, em, am, ew, aw, gray_map);
            break;
        case 5:
            for (int i = 0; i < iterations; i++)
                levels_adjustment_V5(pix_map, width, height, a, b, c, es, as, em, am, ew, aw, gray_map);
            break;
        case 6:
            for (int i = 0; i < iterations; i++)
                levels_adjustment_V6(pix_map, width, height, a, b, c, es, as, em, am, ew, aw, gray_map);
            break;
        default:
            puts("UNREACHABLE!");
    }
    time = curtime() - start;
    if (bflag) printf("Took %f seconds\n", time);

    // Write Image
    write_img(output_img_path, gray_map, width, height, 255, output_flag);

    // Cleanup
    free(gray_map);
    munmap(pix_map, (width * height * 3));
    return EXIT_SUCCESS;


    // TODO: cleanup case mem or img_error
    mem_error:
        fprintf(stderr, "mem_error\n");
        return EXIT_FAILURE;

    img_error:
        fprintf(stderr, "img_error\n");
        return EXIT_FAILURE;

    input_error:
        fprintf(stderr, "Input file does not meet the specification. Try a different file or --help.\n");
        return EXIT_FAILURE;

    output_error:
        fprintf(stderr, "output_error\n");
        return EXIT_FAILURE;

    arg_error:
        fprintf(stderr, "arg_error\n");
        return EXIT_FAILURE;
}
