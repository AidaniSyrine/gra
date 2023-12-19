//
// Created by tade on 12/16/23.
//

//Header Guards
#pragma once

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>

#define ES 0
#define AS 0
#define EW = 255
#define AW 255
#define return_defer(value) do { result = (value); goto defer; } while (0)
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))
#define EXIT_MEM_FAILURE 2


void print_help();
int is_valid_digit(const char* option_arg, int* valid_arg);
int test_io(char* path, const char* arg);
int read_img(const char* img_path, uint8_t* img, size_t* width, size_t* height);
int test_coeff_args(int* dest, char* args[], int position);
int test_lvl_args(int* dest, char* args[], int position);



