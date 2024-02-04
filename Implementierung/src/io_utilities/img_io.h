//
// Created by tade on 12/16/23.
//

#pragma once

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/fcntl.h>

typedef struct Image_params {
    uint8_t color_depth; 
    size_t width; 
    size_t height; 
    size_t image_size;
    uint8_t* pix_map; 
    void* image_ptr; 
} Image_params; 

int read_img(Image_params* image_params, const char* input_path);

int write_img(const char* output_path, const uint8_t* gray_map, Image_params* image_params);

void dealloc_image_params(Image_params* image_params);
