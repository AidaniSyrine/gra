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

#define ES  0      /*Standard Black Input "Eingabe Schwarz"*/
#define AS  0      /*Standard Black Output "Ausgabe Schwarz"*/
#define EW  255    /*Standard White Input "Eingabe Schwarz"*/
#define AW  255    /*Standard White Output "Ausgabe Schwarz"*/
#define DEFAULT_OUTPUT_PATH "./images/output.pgm"

/*  Facilitate Cleanup */
#define return_defer(value) do { result = (value); goto defer; } while (0)

/* Compute the size of array */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

/* Indicate occurrence of memory allocation error */
#define EXIT_MEM_FAILURE 2




/**
 * Test whether the option_arg is a valid digit
 * Convert it to int and store it in valid_arg
 * @param option_arg Option argument
 * @param valid_arg Converted option argument
 * @return EXIT_SUCCESS conversion successful
 * @return EXIT_FAILURE otherwise
 */
int test_and_set_sarg(int* valid_arg, const char* option_arg);


/**
 * __COMMENT__
 * @param valid_args
 * @param option_args
 * @param num_args
 * @return
 */
int test_and_set_largs(float* valid_args, char** option_args, int flag);


/**
 * __COMMENT__
 * @param path
 * @param arg
 * @return
 */
int test_and_set_input(char* path, const char* arg);

int test_and_set_output(char * path, const char* arg);

/**
 * Load the image to the virtual address space
 * Check whether the image has P6 graphic format
 * Read the meta data
 * Point to PixMap
 * @param img_path File's path
 * @param pix_map Pointer to PixMap array
 * @param width  Pointer to PixMap's width in pixels
 * @param height Pointer to PixMap's height in pixels
 * @return EXIT_FAILURE  Image status | Format not compatible
 * @return EXIT_MEM_FAILURE Memory mapping failed
 * @return EXIT_SUCCESS Image read and processed
 * @Note width and height consists of the image
 * dimensions in pixels NOT in Bytes
 */
int read_img(const char* img_path, uint8_t** pix_map, size_t* width, size_t* height, uint8_t* color_depth);


/**
 * __COMMENT__
 * @param img_path
 * @param pix_map
 * @param width
 * @param height
 * @param flag
 * @return
 */
int write_img(const char* img_path, const uint8_t* pix_map, size_t width, size_t height, uint8_t color_depth, int flag);



void print_help(void);

