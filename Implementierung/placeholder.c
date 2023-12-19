//
// Created by tade on 12/16/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

/*
 *
 */
void print_help(){
    puts("print_help()");
}

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
        return 1;
    }
    return 0;
}

/*
 *
 */
int read_img(const char* img_path, uint8_t* img, size_t width, size_t height) {

return 0;

}

/*
 *
 */
int test_lvl_args(double* dest, char* optarg) {
    errno = 0;
    char* endptr;

    dest[0] = strtod(optarg, &endptr);
    
    if (endptr == optarg || *endptr != ',' || errno == ERANGE || dest[0]>255 || dest[0]<0)
        return EXIT_FAILURE;
    errno = 0;
    dest[1] =  strtod(endptr+1, &endptr);
    if ( *endptr != '\0' || errno == ERANGE || dest[1]>255 || dest[1]<0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS; 
}

/*
 *
 */
int test_coeff_args(int* dest, char* args[], int position){
   return 0;
}

