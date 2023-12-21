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


int test_and_set_largs(double* valid_args,const char** option_args, size_t num_args) {
    char* tocken;
    char* endptr;
    size_t i;
    char* rest = NULL;
    char* dup = strdup(*option_args);


    for(tocken = strtok_r(dup, ",", &rest), i = 0;
        tocken != NULL;
        tocken = strtok_r(NULL, ",", &rest), i++)
    {
        errno = 0;
        valid_args[i] = strtod(tocken, &endptr);
        if(endptr == tocken || *endptr != '\0' || errno == ERANGE)
            return EXIT_FAILURE;
    }
    if (i != num_args) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}


int test_and_set_io(char* path, const char* arg) {
    if (!access(arg, F_OK)) {
        strncpy(path, arg, 256);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


int read_img(const char* img_path, uint8_t** pix_map, size_t* width, size_t* height) {

    // Open file
    int result;
    int fd;
    if ((fd = open(img_path, O_RDONLY)) < 0) return_defer(EXIT_FAILURE);

    // Retrieve file stats
    struct stat statbuf;
    if (fstat(fd, &statbuf)) return_defer(EXIT_FAILURE);
    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size <= 0) return_defer(EXIT_FAILURE);

    // Load the file to the virtual address space
    void* img_ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (img_ptr == MAP_FAILED) return_defer(EXIT_MEM_FAILURE);

    // Check image's version
    char * ascii_data = (char *) img_ptr;
    if (ascii_data[0] != 'P' || ascii_data[1] != '6') return_defer(EXIT_FAILURE);
    ascii_data += 2;

    // If exists, ignore comments
     if (ascii_data[0] == '#')
         ascii_data = strchr(ascii_data, '\n');
     ascii_data++;

     // Read width, height, and color depth, assuming them to be valid digits
    uint8_t color_depth;
    if(sscanf(ascii_data, "%zu %zu\n%hhu", width, height, &color_depth) != 3) return_defer(EXIT_FAILURE);  // NOLINT(*-err34-c)
    if (color_depth < 0 || color_depth > 255) return_defer(EXIT_FAILURE);

    // Move the pointer toward the first pixel
    ascii_data = strchr(ascii_data, '\n');
    ascii_data++;
    ascii_data = strchr(ascii_data, '\n');
    ++ascii_data;

    // Cast the pointer to read raw data to bytes
    *pix_map = (uint8_t*) ascii_data;

    if(close(fd) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;

    // Cleanup case Failure
    defer:
    if(close(fd) < 0) return EXIT_FAILURE;
    if (img_ptr >= (void*) 0) munmap(img_ptr, statbuf.st_size);
    return result;
}




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
