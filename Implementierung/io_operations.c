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
        return 1;
    }
    return 0;
}

/*
 *
 */
int read_img(const char* img_path, uint8_t* img, size_t width, size_t height) {



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



/*
int main (){
    int* valid = alloca(sizeof (int));
    char * optargd= "45";
    int result = is_valid_digit(optargd, valid);
    printf("result = %d, valid = %d", result, *valid);
}
 */
