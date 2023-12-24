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

    //Arguments and (Meta) data
    int varg = 0, barg = 0;
    char input_img_path[512], output_img_path[512];
    float a, b, c;
    uint8_t es, as, em, am, ew, aw;


    // Read options
    char valid_args[3];
    while((option = getopt_long(argc, argv, "-HO:V:B::ho:v:b::", longopts, &option_index)) != EOF) {
            switch(option) {
                //check double
                //Next update
                case 0: // Handle longopts
                    switch (longopts[option_index].val) {
                        case 'c':
                            if (test_and_set_largs(valid_args, (const char **) &optarg, cflag)) goto arg_error;
                            a = ((float *) valid_args)[0]; b = ((float *) valid_args)[1]; c = ((float*) valid_args)[2];
                            break;
                        case 's':
                            if (test_and_set_largs(valid_args, (const char **) &optarg, sflag)) goto arg_error;
                            es = ((uint8_t*) valid_args)[0]; as = ((uint8_t*)valid_args)[1];
                            break;
                        case 'm':
                            if (test_and_set_largs(valid_args, (const char **) &optarg, mflag)) goto arg_error;
                            em = ((uint8_t*) valid_args)[0]; am = ((uint8_t*)valid_args)[1];
                            break;
                        case 'w':
                            if (test_and_set_largs(valid_args, (const char **) &optarg, wflag)) goto arg_error;
                            ew = ((uint8_t*) valid_args)[0]; aw = ((uint8_t*)valid_args)[1];
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


    // Setting Default value
    if (!input_flag) goto arg_error;
    if (!sflag) es = ES; as = AS;
    if (!mflag) em = (ES + AW) / 2; am = (AS + AW) / 2;
    if (!wflag) ew = EW; aw =AW;

    // Read image
    size_t width, height;
    uint8_t color_depth, *pix_map = NULL;
    int ret = read_img(input_img_path, &pix_map, &width, &height, &color_depth);
    if (ret == EXIT_MEM_FAILURE) goto mem_error;
    if (ret == EXIT_FAILURE) goto img_error;




    /*
     * a, b, c depens on --coeffs If not, on varg aka. chosen implemtation
     * By definition -v0 is the main entry point
     * Depending on barg
     * Convert image (Gray scale + conversion) then write the image
     */


    write_img(output_img_path, pix_map, width, height, 255, output_flag);

    //Unmap the allocated mem
    munmap(pix_map, (width * height * 3));
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