//
// Created by tade on 12/16/23.
//

#include "io_operations.h"

/*
 *
 */
void print_help(){
    puts("print_help()");
}

/*
 *
 */
int is_valid_digit(const char* option_arg, int* valid_arg) {
    errno = 0;
    char* endptr;

    *valid_arg = (int) strtol(option_arg, &endptr, 10);
    if (endptr == option_arg || *endptr != '\0' || errno == ERANGE)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

/*
 *
 */
int test_io(char* path, const char* arg) {
    if (!access(arg, F_OK)) {
        strncpy(path, arg, 256);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/*
 *
 */
int read_img(const char* img_path, uint8_t* img, size_t* width, size_t* height) {
    printf("input_path %s\n", img_path);

    int result;
    FILE* fd = fopen(img_path, "rb");
    if (!fd) return_defer(EXIT_FAILURE);
    // File must be reg
    struct stat statbuf;
    if (stat(img_path, &statbuf)) return_defer(EXIT_FAILURE);
    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size <= 0) return_defer(EXIT_FAILURE);


    //img version
    char img_version[2];
    size_t ret = fread(img_version, sizeof (*img_version), ARRAY_SIZE(img_version), fd);
    if (ret != ARRAY_SIZE(img_version)) return_defer(EXIT_FAILURE);
    if (!strncmp(img_version, "P6", 2)) return_defer(EXIT_FAILURE);
    int color_depth = 0;
    fscanf(fd, "   %zu %zu   ", width, height);
    fscanf(fd, "  %i  ", &color_depth);

    if(color_depth < 0 || color_depth > 255) return_defer(EXIT_FAILURE);

    img = (uint8_t*) malloc((*height) * (*width) * sizeof (uint8_t));
    if (!img) return_defer(EXIT_MEM_FAILURE);

    //Future update using SIMD and checks for errors
    for (size_t i = 0; i < (*width) * (*height); i++) {
            fscanf(fd, "%sCNu8", (img + i));
    }
    return_defer(EXIT_SUCCESS);

    defer:
        if (!img) free(img);
        if (fd) fclose(fd);
        if (errno) return EXIT_FAILURE;
        return result;
}

/*
 *
 */
int test_lvl_args(int* dest, char* args[], int position) {
    return 0;
}

/*
 *
 */
int test_coeff_args(int* dest, char* args[], int position){
    return 0;
}
