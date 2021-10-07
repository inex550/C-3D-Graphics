#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tga/tga.h"
#include "inex/std.h"
#include "inex/array.h"
#include "graphics/graphics.h"
#include <time.h>
    
#define OUT_TGA "out.tga"

TGAColor white = { 0xff, 0xff, 0xff, 0 };
TGAColor red = { 0x00, 0x00, 0xff, 0 };
TGAColor green = { 0x00, 0xff, 0x00, 0 };
TGAColor blue = { 0xff, 0x00, 0x00, 0 };

void rasterize(TGAImage *img, Vect2i v0, Vect2i v1, int ybuffer[], TGAColor color, int height) {
    if (v0.x > v1.x) swap(v0, v1, Vect2i);

    int dx = v1.x-v0.x;

    for (int x = v0.x; x < v1.x; x++) {
        float alpha = (float)(x-v0.x)/dx;
        int y = v0.y + (v1.y - v0.y)*alpha;
        //int y = v0.y*(1.-alpha) + v1.y*alpha;

        if (ybuffer[x] < y) {
            ybuffer[x] = y;

            for (int iy = 0; iy < height; iy++)
                tga_set(img, x, iy, color);
        }
    }
}

int main(int argc, char *argv[]) {

    srand(time(NULL));

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int width = 2000;
    int height = 2000;

    char *filename = argv[1];

    TGAImage *img = tga_create(width, height, RGB);
    if (img == NULL) {
        fprintf(stderr, "TGAImage error");
        return 1;
    };

    int nbytes = width * height * sizeof(int);
    int *zbuffer = malloc(nbytes);
    memset(zbuffer, CHAR_MIN, nbytes);

    draw_model_faces(img, filename, (Vect3f){0.2, 0.2, -1}, 300, zbuffer);
    //draw_model_edges(img, filename, white);

    write_tga_file(OUT_TGA, img);
    tga_free(img);
}