//
// Created by Adem Trabelsi on 12.01.24.
//
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h>
#include <inttypes.h> 
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>



#define ES  0      
#define AS  0    
#define EW  255    
#define AW  255 
#define A .2126
#define B .7152
#define C .0722
#define MAIN_VERSION 0  
#define DEFAULT_ITER_NUM 3
#define DEFAULT_OUTPUT_PATH "../resources/default_output.pgm"

typedef struct Input_params {
    bool enable_help; 
    bool enable_tests;
    float a, b, c;
    float es, as; 
    float em, am; 
    float ew, aw;
    int version;
    int iter_num; 
    char* input_img_path;
    char* output_img_path; 
} Input_params;

static int cflag = 0, sflag = 0, mflag = 0, wflag =0;

static const struct option longopts[] = {
        {"coeffs",  required_argument, &cflag, 'c'},
        {"lvlss",   required_argument, &sflag, 's'},
        {"lvlsm",   required_argument, &mflag, 'm'},
        {"lvlsw",   required_argument, &wflag, 'w'},
        {"test",    no_argument, 0, 't'},
        {"help",    no_argument, 0, 'h'},
        {0,0,0,0}
};

int test_and_set_sarg(int* valid_arg, const char* option_arg);

int test_and_set_largs(float* valid_args, char* option_args, int flag);

int test_and_set_input(Input_params* input_params, const char* arg);

int test_and_set_output(Input_params* input_params, const char* arg);

int arg_parser(Input_params* input_params, int argc, char* argv[]); 

void dealloc_input_params(Input_params* input_params);

