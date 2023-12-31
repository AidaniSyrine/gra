//
// Created by tade on 12/16/23.
//

#include "io_operations.h"



int test_and_set_sarg(int* valid_arg, const char* option_arg) {
    errno = 0;
    char* endptr;

    *valid_arg = (int) strtol(option_arg, &endptr, 10);
    if (endptr == option_arg || *endptr != '\0' || errno == ERANGE)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int test_and_set_largs(float* valid_args, char** option_args, int flag) {
    char* token = NULL;
    char* endptr;
    size_t i;
    for(token = strtok(*option_args, ","), i = 0;
        token != NULL; 
        token = strtok(NULL, ",")) {
            errno = 0;
            float tmp = strtof(token, &endptr);
             printf("token = %s\n", token);
            if(endptr == token || *endptr != '\0' ||errno == ERANGE)
            return EXIT_FAILURE;
            printf("%f\n", tmp);
            if (flag == 'c') {
                if (i > 2 || tmp < 0) return EXIT_FAILURE;
                valid_args[i] = tmp;
                printf("%f\n", valid_args[i]);

            } else {
                if (i > 1 || tmp > 255 || tmp < 0) return EXIT_FAILURE;
                valid_args[i] = tmp;
                printf("%f\n",  valid_args[i]);
            }
        }
}
/*
int test_and_set_largs(void* valid_args, char** option_args, int flag) {
    char* token;
    char* endptr;
    size_t i;
    char* rest = NULL;
    char* dup = strdup(*option_args);


    for(token = strtok_r(dup, ",", &rest), i = 0;
        token != NULL;
        token = strtok_r(NULL, ",", &rest), i++)
    {
        errno = 0;
        float tmp = strtof(token, &endptr);

        if(endptr == token || *endptr != '\0' ||errno == ERANGE)
            return EXIT_FAILURE;

        if (flag == 'c') {
            if (i > 2 || tmp < 0) return EXIT_FAILURE;
            ((float *) valid_args)[i] = tmp;
            printf("%f\n", ((float *) valid_args)[i]);

        } else {
            if (i > 1 || tmp > 255 || tmp < 0) return EXIT_FAILURE;
            ((uint8_t *) valid_args)[i] = (uint8_t) tmp;
            printf("%f\n", ((float *) valid_args)[i]);
        }
    }
    
    
    return EXIT_SUCCESS;

}
*/


int test_and_set_input(char* path, const char* arg) {
    // Input file should exist and be accessible
    if (!access(arg, F_OK)) {
        strncpy(path, arg, 512);
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "File does not exist or is not accessible: %s.\n", arg);
    return EXIT_FAILURE;
}


// Ouput must be pgm file
int test_and_set_output(char* path, const char* arg) {
    size_t suffix_len = strlen(".pgm");
    size_t arg_len = strlen(arg);

    // .pgm as an output file name is not allowed
    if (arg_len < suffix_len + 1) return EXIT_FAILURE;

    // None pgm files are also not allowed
    if (!strncmp(arg + arg_len - suffix_len, ".pgm", suffix_len)) {
        strncpy(path, arg, 512);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


int read_img(const char* img_path, uint8_t** pix_map, size_t* width, size_t* height, uint8_t* color_depth) {

    // Open file
    int result;
    int fd;
    if ((fd = open(img_path, O_RDONLY)) < 0) {
        fprintf(stderr, "Cannot open the file %s.\n", img_path);
        return_defer(EXIT_FAILURE);
    }
  
    // Retrieve file stats
    struct stat statbuf;
    if (fstat(fd, &statbuf)) {
        fprintf(stderr, "fstat() failed: %s.\n", strerror(errno));
        return_defer(EXIT_FAILURE);
    }
    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size <= 0) {
        fprintf(stderr, "Provided file: %s is an empty or non regular file.\n", img_path);
        return_defer(EXIT_FAILURE);
    }
    // Load the file to the virtual address space
    void* img_ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (img_ptr == MAP_FAILED) return_defer(EXIT_MEM_FAILURE);

    // Check image's version
    char * ascii_data = (char *) img_ptr;
    if (ascii_data[0] != 'P' || ascii_data[1] != '6') {
        fprintf(stderr, "Provided file:\"%s\"is not a PPM (P6) image.\n", img_path);
        return_defer(EXIT_FAILURE);
    }
    ascii_data += 2;

    // If exists, ignore comments
     while (ascii_data[0] == '#')
         ascii_data = strchr(ascii_data, '\n');
     ascii_data++;

     // Read width, height, and color depth
    int depth_tmp;
    if(sscanf(ascii_data, "%zu %zu\n%d", width, height, &depth_tmp) != 3) { 
        fprintf(stderr, "Provided file: %s is not a regular PPM image.\n", img_path);
        return_defer(EXIT_FAILURE); 
    }
    if (depth_tmp < 0 || depth_tmp > 255) {
        fprintf(stderr, "Provided file: %s is not a regular PPM image.\n", img_path);
         return_defer(EXIT_FAILURE);
    }
    *color_depth = (uint8_t) depth_tmp;

    // Move the pointer toward the first pixel
    ascii_data = strchr(ascii_data, '\n');
    ascii_data++;
    ascii_data = strchr(ascii_data, '\n');
    ++ascii_data;

    // Cast the pointer to read raw data to bytes
    *pix_map = (uint8_t*) ascii_data;

    if(close(fd) < 0) {
        fprintf(stderr, "close() failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

    // Cleanup case Failure
    defer:
    if(close(fd) < 0) {
        fprintf(stderr, "close() failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (img_ptr != NULL) munmap(img_ptr, statbuf.st_size);
    return result;
}


int write_img(const char *img_path, const uint8_t* pix_map,  size_t width, size_t height, uint8_t color_depth, int flag) {
    // Open file
    int result;
    FILE *f;
    if (!flag) img_path = DEFAULT_OUTPUT_PATH;
    f = fopen(img_path, "wb");
    if (!f) return_defer(EXIT_FAILURE);

    // Write the corresponding header
    fprintf(f, "P5\n%zu %zu\n%i\n", width, height, color_depth);
    if(ferror(f)) return_defer(EXIT_FAILURE);

    // Load pix_map to the file
    fwrite(pix_map, sizeof(uint8_t), height * width, f);
    if(ferror(f)) return_defer(EXIT_FAILURE);

    //Cleanup
    defer:
    if (f) fclose(f);
    if (errno) return EXIT_FAILURE;
    return result;
}


// TODO
void print_help(void){
    puts("\n"
         "Desciption:\n"
         "--to edit--\n"
         "Usage:\n"
         "      tonwertkorrktur <input_image>\n"
         "NOTE: *.ppm                           Input image MUST have a 24bpp PPM Format (P6)\n"
         "\n"
         "Optional Arguments:\n"
         "  -V<int>                             ---\n"
         "  -B<int>                             ---\n"
         "  -o<output_img>                      ---\n"
         "NOTE:\n"
         " *.ppm                                Output image MUST have a 24bpp PPM Format (P6).\n"
         "  --coeffs<double>,<double>,<double>  ---\n"
         "  --lvlss<double>,<double>            ---\n"
         "  --lvlsm<double>,<double>            ---\n"
         "  --lvlsw<double>,<double>            ---\n"
         "  -h|--help                           ---\n"
         );
}
