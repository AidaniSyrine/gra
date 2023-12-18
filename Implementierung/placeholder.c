//
// Created by tade on 12/16/23.
//
#include <stdio.h>

void print_help(){
    puts("print_help()");
}

struct ppm {
    size_t width;
    size_t height; 
    int maxpixel;
};

unsigned char*  isvalid (FILE *input_file , struct ppm* inputdim){
    char identifier[2];
    if (fgets(identifier,sizeof(identifier),input_file)==NULL){
        printf(stderr,"Error reading file\n");
        return NULL;
    }
    if (strcmp (identifier,"P6",2)!=0){
        printf(stderr,"file type not PPM\n");
        return NULL;
    }
    if (fscanf(input_file, "%d %d %d", &inputdim->width, &inputdim->height, &inputdim->maxpixel)!=3)
    {
        printf(stderr,"Error reading dimensions\n");
        return NULL;
    }
    if (inputdim->width <=0 ||inputdim->height <=0 || inputdim->maxpixel <=0 || inputdim->maxpixel > 255){
        printf(stderr,"incorrect dimensions\n");
        return NULL;
    }
    unsigned char *pixel_val =(unsigned char *) malloc(1+(inputdim->width*inputdim->height*3));
    size_t count = fread(pixel_val, 1,inputdim->width*inputdim->height*3, input_file );
    if (count != inputdim->width*inputdim->height*3){
        printf(stderr,"incorrect number of pixels\n");
        free(pixel_val);
        return NULL;
    }
    for (size_t i = 0; i < count; i++){
        if (pixel_val[i]>255 ){
            printf(stderr,"pixel value greater than %d\n", inputdim->maxpixel);
            free(pixel_val);
            return NULL; 
        }
    }
    return pixel_val;
}

void greylevels (){

}