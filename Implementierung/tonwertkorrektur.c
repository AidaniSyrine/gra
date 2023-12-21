//
// Created by tade on 12/16/23.
//
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>
#include "io_operations.h"
 
#include "io_operations.h"


static int cflag = 0, sflag = 0, mflag = 0, wflag =0;
static const struct option longopts[] = {
        {"coeffs",  required_argument, &cflag, 'c'},
        {"lvlss",   required_argument, &sflag,  's'},
        {"lvlsm",   required_argument, &mflag, 'm'},
        {"lvlsw",   required_argument, &wflag, 'w'},
        {"help",    no_argument, 0, 'h'},
        {0,0,0,0}
};


void levels_adjustment(
        const uint8_t* img, size_t width, size_t height,
        float a, float b, float c,
        uint8_t es, uint8_t as, uint8_t em, uint8_t am,
        uint8_t ew, uint8_t aw,
        uint8_t* result
        );

      


int main(int argc, char* argv[]) {
    // get-opt variables
    int option_index = 0;
    int option;

    // Flags
    int input_flag = 0, output_flag = 0, vflag = 0, bflag = 0, hflag = 0;
    // Flags
    int input_flag = 0, output_flag = 0, vflag = 0, bflag = 0;

    // Arguments
    int varg = 0, barg = 1;
    char input_img_path[256];
    char output_img_path[256];
    double a, b, c , es, as, em, am, ew, aw;
    //Arguments
    int varg = 0, barg = 0;
    char input_img_path[512];
    char output_img_path[512];
    double valid_args[3];
    double a, b, c, es, as, em, am, ew, aw;

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
   // Read options
    while((option = getopt_long(argc, argv, "-HO:V:B::ho:v:b::", longopts, &option_index)) != EOF) {
            switch(option) {
                //check double
                case 0: //Handle longopts
                //check double
                //Next update
                case 0: // Handle longopts
                    switch (longopts[option_index].val) {
                        case 'c':
                            ;double coeff[3];
                            if (test_args(coeff, optarg,3))
                                goto arg_error;
                            a = coeff[0];
                            b = coeff[1];
                            c = coeff[2];
                            if (test_and_set_largs(valid_args, (const char **) &optarg, 3)) goto arg_error;
                            a = valid_args[0]; b = valid_args[1]; c = valid_args[2];
                            break;
                        case 's':
                            ;double es_as[2];
                            if (test_args(es_as, optarg,2))
                               goto arg_error;
                            es = es_as[0];
                            as = es_as[1];
                            if (test_and_set_largs(valid_args, (const char **) &optarg, 2)) goto arg_error;
                            es = valid_args[0]; as = valid_args[1];
                            break;
                        case 'm':
                            ;double em_am[2];
                            if (test_args(em_am, optarg,2))
                                goto arg_error;
                            em = em_am[0];
                            am = em_am[1];
                            if (test_and_set_largs(valid_args, (const char **) &optarg, 2)) goto arg_error;
                            em = valid_args[0]; am = valid_args[1];
                            break;
                        case 'w':
                            ;double ew_aw[2];
                            if (test_args(ew_aw, optarg,2))
                                goto arg_error;
                            ew = ew_aw[0];
                            aw = ew_aw[1];
                            if (test_and_set_largs(valid_args, (const char **) &optarg, 2)) goto arg_error;
                            ew = valid_args[0]; aw = valid_args[1];
                            break;
                        case '?':
                            puts("UNREACHABLE");
                            return EXIT_FAILURE;
                    } break;
                case 1: // Non-option arg input file
                    if(input_flag) goto arg_error;
                    if (test_and_set_io(input_img_path, optarg)) goto io_error;
                    input_flag++;
                    printf("input = %s\n", input_img_path);
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
                case 'o': case 'O':
                    if (output_flag) goto arg_error;
                    if (test_and_set_io(output_img_path, optarg)) goto io_error;
                    output_flag++;
                    printf("output = %s\n", output_img_path);
                    break;
                case 'V': case 'v':
                    if (vflag || test_and_set_sarg(&varg, optarg)) goto arg_error;
                    vflag++;
                    printf("varg= %d\n", varg);
                    break;
                case 'B': case 'b':
                    if (bflag) goto arg_error;
                    bflag++;
                    if (!optarg) {
                        barg = 1;
                        printf("barg= %d\n", barg);
                        break;
                    }
                    if (test_and_set_sarg(&barg, optarg)) goto arg_error;
                    printf("barg= %d\n", barg);
                    break;
                case 'h': case 'H':
                    print_help();
                    return EXIT_SUCCESS;
                case '?': default:
                    goto arg_error;

            }
    }

    // read image
    size_t width, height;
    uint8_t *img_ptr = NULL;
    int ret = read_img(input_img_path, &img_ptr, &width, &height);
    if (ret == EXIT_MEM_FAILURE) goto mem_error;
    if (ret == EXIT_FAILURE) goto img_error;
    if (cflag) {
        printf("a= %f, b= %f ,c= %f\n", a, b, c);
    }
    if (sflag) {
        printf("es= %f, as= %f\n", es, as);
    }
    if (wflag) {
        printf("ew= %f, aw= %f\n", ew, aw);
    }
    if(mflag) {
        printf("em= %f, am= %f\n", em, am);
    }


    // Next Update Test whether the whole mmap freed or just the PixMap
    // Should work both are in the same page
    munmap(img_ptr, (width * height * 3));
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
