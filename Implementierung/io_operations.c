//
// Created by tade on 12/16/23.
//
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 *
 */
void print_help(){
    puts("print_help()");
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
int read_img(const char* path, uint8_t* img, size_t width, size_t height) {
    return 0;
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

