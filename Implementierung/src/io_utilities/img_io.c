//
// Created by tade on 12/16/23.
//

#include "img_io.h"

int read_img(Image_params* image_params, const char* input_path) {
    
    // Open file
    int fd;
    if ((fd = open(input_path, O_RDONLY)) < 0) {
       
        fprintf(stderr, "Failed! Cannot open input file %s.\n%s.\n", input_path, strerror(errno));
        return EXIT_FAILURE;
    }

    // Retrieve file stats
    struct stat statbuf;
    if (fstat(fd, &statbuf)) {
        fprintf(stderr, "Failed! %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size <= 0) {
        fprintf(stderr, "Failed! Provided file: %s is an empty or non regular.\n", input_path);
        return EXIT_FAILURE; 
    }
    
    // Load the file to the virtual address space
    void* img_ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (img_ptr == MAP_FAILED) {
        fprintf(stderr, "Failed! Could not load the file to memory.\n"); 
        return EXIT_FAILURE;
    } 

    // Needed for Unmapping
    image_params->image_ptr = img_ptr; 
    image_params->image_size = statbuf.st_size;

    // Check image's version 
    
    char * ascii_data = (char *) img_ptr;
    if (ascii_data[0] != 'P' || ascii_data[1] != '6') {
        fprintf(stderr, "Failed! Provided file: %s is not a PPM (P6) image.\n", input_path);
        return EXIT_FAILURE;
    }
    ascii_data += 3;

    // If exists, ignore comments
    
    while (ascii_data[0] == '#') {
        ascii_data = strchr(ascii_data, '\n');
        ascii_data++;
    }

    // Read width, height, and color depth
    int depth_tmp;
    if(sscanf(ascii_data, "%zu %zu\n%d", &image_params->width, &image_params->height, &depth_tmp) != 3) { 
        fprintf(stderr, "Failed! Provided file: %s is not a regular PPM image. Non valid metadata.\n", input_path);
        return EXIT_FAILURE; 
    } 

    if (depth_tmp < 0 || depth_tmp > 255) {
        fprintf(stderr, "Failed! Provided file: %s is not a regular PPM image. Non valid depth.\n", input_path);
        return EXIT_FAILURE;
    }
    image_params->color_depth = (uint8_t) depth_tmp;

    // Move the pointer toward the first pixel
    ascii_data = strchr(ascii_data, '\n');
    ascii_data++;
    ascii_data = strchr(ascii_data, '\n');
    ascii_data++;
    
    // Check Pix_map 
    size_t metadata_len = strlen(img_ptr) - strlen(ascii_data); 
    if ((size_t)statbuf.st_size !=  image_params->width * image_params->height * 3 + metadata_len) {
        fprintf(stderr, "Failed! Provided file %s is not a regular PPM image. Number of pixels is ambiguous.\n", input_path);
        return EXIT_FAILURE; 
    }

    // Cast the pointer to read raw data to bytes
    image_params->pix_map = (uint8_t*) ascii_data;

    if(close(fd) < 0) {
        fprintf(stderr, "Failed! %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int write_img(const char* output_path, const uint8_t* gray_map, Image_params* image_params) {
    // Open file
    FILE *f;
    f = fopen(output_path, "wb");
    if (!f) {
        fprintf(stderr, "Failed! Cannot open output file %s\n", output_path);
        return EXIT_FAILURE;
    }

    // Write the corresponding header
    fprintf(f, "P5\n%zu %zu\n%i\n", image_params->width, image_params->height, image_params->color_depth);
    if(ferror(f)) goto handle_error;

    // Load pix_map to the file
    size_t written = fwrite(gray_map, sizeof(uint8_t), image_params->width * image_params->height, f);
    if(ferror(f) || written < (image_params->width * image_params->height)) goto handle_error;

    //Cleanup 
    fclose(f); 
    return EXIT_SUCCESS; 
    
    handle_error:
        fprintf(stderr, "Failed! Writing to the output file.\n"); 
        fclose(f);
        return EXIT_FAILURE;
}

void dealloc_image_params(Image_params* image_params) {
    errno = 0;
    int ret; 
    if(image_params->pix_map != NULL) {
        if ((ret = munmap(image_params->image_ptr, image_params->image_size))) {
            printf("ret = %d\n", ret);
            fprintf(stderr, "Failed! Unable to free the PPM image. %s\n", strerror(errno)); 
        }
    }
}
