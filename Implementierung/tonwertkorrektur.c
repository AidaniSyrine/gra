//
// Created by tade on 12/16/23.
//
#include <getopt.h>
#include "io_operations.h"


static int cflag = 0, sflag = 0, mflag = 0, wflag =0;
static const struct option longopts[] = {
        {"coeffs",  required_argument, 0, 'c'},
        {"lvlss",   required_argument, 0,  's'},
        {"lvlsm",   required_argument, 0, 'm'},
        {"lvlsw",   required_argument, 0, 'w'},
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
    int input_flag = 0, output_flag = 0, vflag = 0, bflag = 0;

    //Arguments
    int varg = 0, barg = 0;
    char input_img_path[512];
    char output_img_path[512];
    float valid_args[3];
    float a, b, c;
    uint8_t es, as, em, am, ew, aw;

   // Read options
    while((option = getopt_long(argc, argv, "-HO:V:B::ho:v:b::c:s:m:w:", longopts, &option_index)) != EOF) {
            switch(option) {
                //longopt
                case 'c':
                    if (cflag || test_and_set_largs(valid_args, (const char **) &optarg, 3)) goto arg_error;
                    cflag++;
                    a = valid_args[0]; b = valid_args[1]; c = valid_args[2];
                    break;
                case 's':
                    if (sflag || test_and_set_largs(valid_args, (const char **) &optarg, 2)) goto arg_error;
                    sflag++;
                    es = valid_args[0]; as = valid_args[1];
                    break;
                case 'm':
                    if (mflag || test_and_set_largs(valid_args, (const char **) &optarg, 2)) goto arg_error;
                    mflag++;
                    em = valid_args[0]; am = valid_args[1];
                    break;
                case 'w':
                    if (wflag || test_and_set_largs(valid_args, (const char **) &optarg, 2)) goto arg_error;
                    wflag++;
                    ew = valid_args[0]; aw = valid_args[1];
                    break;
                //Next update
                case 1: // Non-option arg input file
                    if(input_flag) goto arg_error;
                    if (test_and_set_io(input_img_path, optarg)) goto io_error;
                    input_flag++;
                    printf("input = %s\n", input_img_path);
                    break;
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
    uint8_t color_depth, *img_ptr = NULL;
    int ret = read_img(input_img_path, &img_ptr, &width, &height, &color_depth);
    if (ret == EXIT_MEM_FAILURE) goto mem_error;
    if (ret == EXIT_FAILURE) goto img_error;
    if (cflag) {
        printf("a= %f, b= %f ,c= %f\n", a, b, c);
    }
    if (sflag) {
        printf("es= %hhu, as= %hhu\n", es, as);
    }
    if (wflag) {
        printf("ew= %hhu, aw= %hhu\n", ew, aw);
    }
    if(mflag) {
        printf("em= %hhu, am= %hhu\n", em, am);
    }

    //write image
    write_img(output_img_path, img_ptr, width, height,255,output_flag);

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