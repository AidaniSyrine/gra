
#include "test_similarity.h"




int testImageSimilarity(uint8_t* image1, uint8_t* image2,size_t size){
    for(size_t i=0;i<size;i++){
        if (abs(image1[i]-image2[i])>1){
            return 0;
        }
    }
    return 1;
}

void test_grayscale_SIMD(Image_params* pix_map){
    float a=1;
    float b=1;
    float c=1;
    size_t size = pix_map->width*pix_map->height;
    uint8_t* gray_map1 = (uint8_t *) malloc(size);
    img_to_gray_scale(gray_map1,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    uint8_t* gray_map2 = (uint8_t *) malloc(size);
    img_to_gray_scale_SIMD(gray_map2,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    assert(testImageSimilarity(gray_map1,gray_map2,size)==1);
    free(gray_map1);
    free(gray_map2);
}

void test_linear_SIMD(Image_params* pix_map){
    float a=.2126;
    float b=.7152;
    float c=.0722;
    size_t size = pix_map->width*pix_map->height;
    float es =80;
    float as =80;
    float em =127.5;
    float am =127.5;
    float ew =140;
    float aw =140;
    uint8_t* gray_map1 = (uint8_t *) malloc(size);
    uint8_t* gray_map2 = (uint8_t *) malloc(size);
    img_to_gray_scale(gray_map1,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    linear_interpolation(gray_map1,pix_map->width,pix_map->height,es,as,em,am,ew,aw);
    img_to_gray_scale(gray_map2,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    linear_interpolation_SIMD(gray_map2,pix_map->width,pix_map->height,es,as,em,am,ew,aw);

    assert(testImageSimilarity(gray_map1,gray_map2,size)==1);
    free(gray_map1);
    free(gray_map2); 
}

void test_quadraticLS_SIMD(Image_params* pix_map){
    float a=.2126;
    float b=.7152;
    float c=.0722;
    size_t size = pix_map->width*pix_map->height;
    float es =80;
    float as =80;
    float em =127.5;
    float am =127.5;
    float ew =140;
    float aw =140;
    uint8_t* gray_map1 = (uint8_t *) malloc(size);
    uint8_t* gray_map2 = (uint8_t *) malloc(size);
    int outofbound = is_out_of_bound(es,as,em,am,ew,aw);
    img_to_gray_scale(gray_map1,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    quadratic_interpolation_Gaussian(gray_map1,pix_map->width,pix_map->height,es,as,em,am,ew,aw,outofbound);
    img_to_gray_scale(gray_map2,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    quadratic_interpolation_Gaussian_SIMD(gray_map2,pix_map->width,pix_map->height,es,as,em,am,ew,aw,outofbound);

    assert(testImageSimilarity(gray_map1,gray_map2,size)==1);
    free(gray_map1);
    free(gray_map2);  
}


void test_newton_SIMD(Image_params* pix_map){
    float a=.2126;
    float b=.7152;
    float c=.0722;
    size_t size = pix_map->width*pix_map->height;
    float es =80;
    float as =80;
    float em =127.5;
    float am =127.5;
    float ew =140;
    float aw =140;
    uint8_t* gray_map1 = (uint8_t *) malloc(size);
    uint8_t* gray_map2 = (uint8_t *) malloc(size);
    int outofbound = is_out_of_bound(es,as,em,am,ew,aw);
    img_to_gray_scale(gray_map1,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    quadratic_interpolation_Newton(gray_map1,pix_map->width,pix_map->height,es,as,em,am,ew,aw,outofbound);
    img_to_gray_scale(gray_map2,pix_map->pix_map,pix_map->width,pix_map->height,a,b,c);
    quadratic_interpolation_Newton_SIMD(gray_map2,pix_map->width,pix_map->height,es,as,em,am,ew,aw,outofbound);

    assert(testImageSimilarity(gray_map1,gray_map2,size)==1);
    free(gray_map1);
    free(gray_map2);  
}


void test_quadratic(Image_params* pix_map) {
    size_t size = pix_map->width*pix_map->height;
    uint8_t* gray_map1 = (uint8_t *) malloc(size);
    levels_adjustment_V2(pix_map->pix_map, pix_map->width, pix_map->height,0.2126,0.7152,0.0722, 0, 0, 127, 127, 255, 255, gray_map1);
    uint8_t* gray_map2 = (uint8_t *) malloc(size);
    levels_adjustment_V3(pix_map->pix_map, pix_map->width, pix_map->height, 0.2126,0.7152,0.0722, 0, 0, 127, 127, 255, 255, gray_map2);
    uint8_t* gray_map3 = (uint8_t *) malloc(size);
    levels_adjustment_V4(pix_map->pix_map, pix_map->width, pix_map->height, 0.2126,0.7152,0.0722, 0,0,127, 127, 255, 255, gray_map3);
    assert(testImageSimilarity(gray_map1,gray_map2,size)==1);
    assert(testImageSimilarity(gray_map2,gray_map3,size)==1);
    free(gray_map1);
    free(gray_map2);
    free(gray_map3);
}

