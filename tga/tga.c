#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tga.h"

void reverse_horizontally(TGAImage img);
void reverse_verticaly(TGAImage img);

#define get_color_ptr(data, width, x, y, cblen) (data + (width*y+x)*cblen)

TGAImage* tga_create(int width, int height, short cblen) {
    TGAImage *img = malloc(sizeof(TGAImage));
    if (img == NULL) return NULL;

    img->width = width;
    img->height = height;
    img->cblen = cblen;

    unsigned long nbytes = width * height * cblen;
    img->data = malloc(nbytes);
    if (img->data == NULL) {
        free(img);
        return NULL;
    }
    memset(img->data, 0, nbytes);

    return img;
}

TGAColor tga_get(TGAImage *tga_img, int x, int y) {
    TGAColor color;
    
    if (tga_img == NULL || x<0 || y<0 || x>=tga_img->width || y>=tga_img->height) {
        color.val = 0;
        return color;
    }

    color.val = *(unsigned long*)(tga_img->data + (x + y * tga_img->width) * tga_img->cblen);
    return color;
}

int tga_set(TGAImage *tga_img, int x, int y, TGAColor color) {
    if (tga_img == NULL || x<0 || y<0 || x>=tga_img->width || y>=tga_img->height)
        return 0;

    void *ptr = tga_img->data + (x + y * tga_img->width) * tga_img->cblen;
    memcpy(ptr, &color, tga_img->cblen);
}

TGAColor make_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return (TGAColor){ b, g, r, a };
}

TGAImage* read_tga_file(char *filename) {

    FILE *tga_file = fopen(filename, "rb");
    if (tga_file == NULL) return NULL;

    TGAImage *tga_img = malloc(sizeof(TGAImage));

    struct tga_header header;
    
    fgets((char*)&header, sizeof header, tga_file);

    if (header.img_width <= 0 || header.img_height <= 0 || header.bitsperpixel <= 0) {
        fclose(tga_file);
        return NULL;
    }

    tga_img->cblen = header.bitsperpixel>>3;
    tga_img->width = header.img_width;
    tga_img->height = header.img_height;

    unsigned long nbytes = header.img_width*header.img_height*tga_img->cblen;
    tga_img->data = malloc(nbytes);

    fread(tga_img->data, 1, nbytes, tga_file);

    fclose(tga_file);

    if (header.img_descriptor & 0x10) {
        reverse_horizontally(*tga_img);
    }

    if (!(header.img_descriptor & 0x20)) {
        reverse_verticaly(*tga_img);
    }

    return tga_img;
}

int write_tga_file(char *filename, TGAImage *img) {

    FILE *fptr = fopen(filename, "wb");
    if (fptr == NULL) return 0;

    struct tga_header header;
    memset(&header, 0, sizeof header);

    header.img_type = 2;
    header.img_width = img->width;
    header.img_height = img->height;
    header.bitsperpixel = img->cblen<<3;
    header.img_descriptor = 0;

    fwrite(&header, sizeof header, 1, fptr);

    unsigned long nbytes = img->width * img->height * img->cblen;
    fwrite(img->data, 1, nbytes, fptr);

    for (int i = 0; i < 16; i++)
        fputc(0, fptr);

    char *singature = "TRUEVISION-XFILE";
    fwrite(singature, 1, 16, fptr);

    fputc('.', fptr);
    fputc(0, fptr);

    return 1;
}

void tga_free(TGAImage *tga_img) {
    free(tga_img->data);
    free(tga_img);
}

void reverse_horizontally(TGAImage img) {

    long *lptr, *rptr, temp;

    short width_half = img.width / 2;

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < width_half; x++) {
            lptr = (long*)get_color_ptr(img.data, img.width, x, y, img.cblen);
            rptr = (long*)get_color_ptr(img.data, img.width, img.width-x-1, y, img.cblen);
            temp = *lptr;
            *lptr = *rptr;
            *rptr = temp;
        }
    }
}

void reverse_verticaly(TGAImage img) {

    unsigned long row_nbytes = img.width * img.cblen;
    char *temp = malloc(row_nbytes);

    short height_half = img.height / 2;
    char *tptr, *bptr;

    for (int row = 0; row < height_half; row++) {
        tptr = img.data + row_nbytes * row;
        bptr = img.data + row_nbytes * (img.height-row-1);
        memcpy(temp, tptr, row_nbytes);
        memcpy(tptr, bptr, row_nbytes);
        memcpy(bptr, temp, row_nbytes);
    }
}