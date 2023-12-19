//
// Created by tade on 12/16/23.
//

//Header Guards
#pragma once

#include <inttypes.h>

const uint8_t ES = 0;
const uint8_t AS = 0;
const uint8_t EW = 255;
const uint8_t AW = 255;

void print_help();
int is_valid_digit(const char* option_arg, int* valid_arg);
int test_io(char* path, const char* arg);
int read_img(const char* img_path, uint8_t* img, size_t width, size_t height);
int test_coeff_args(int* dest, char* args[], int position);
int test_lvl_args(int* dest, char* args[], int position);



