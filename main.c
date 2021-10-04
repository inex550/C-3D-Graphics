#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tga/tga.h"
#include "model/model.h"
#include "inex/array.h"

#define swap(a, b) { \
    a = a ^ b; \
    b = a ^ b; \
    a = b ^ a; \
}

#define OUT_TGA "out.tga"

TGAColor white = { 0xff, 0xff, 0xff, 0 };

void draw_line(TGAImage *img, int x0, int y0, int x1, int y1, TGAColor color) {

    char trasp = 0;
    if (abs(x0 - x1) < abs(y0-y1)) {
        swap(x0, y0);
        swap(x1, y1);

        trasp = 1;
    }

    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int dx = (x1-x0);
    int dy = (y1-y0);
    int derror = abs(dy);
    int error = 0;
    int y = y0; 

    for (int x = x0; x <= x1; x++) {
        if (trasp)
            tga_set(img, y, x, color);
        else
            tga_set(img, x, y, color);

        error += derror;

        if (error > dx) {
            y += (y1>y0?1:-1);
            error -= dx;
        }
    } 
}

int main() {

    int width = 1000;
    int height = 1000;

    TGAImage *img = tga_create(width, height, RGB);

    Model *model = read_obj_model("res/Pistol_2.obj");

    for (int iface = 0; iface < model->nfaces; iface++) {
        Face face = model->faces[iface];

        for (int i = 0; i < face.nedges; i++) {
            Vect3f v0 = model->vects[face.edges[i].v-1];
            Vect3f v1 = model->vects[face.edges[(i+1)%face.nedges].v-1];
            
            int x0 = (v0.x+1.)*width/3.;
            int y0 = (v0.y+1.)*height/3.;
            int x1 = (v1.x+1.)*width/3.;
            int y1 = (v1.y+1.)*height/3.;

            draw_line(img, x0, y0, x1, y1, white);
        }
    }

    write_tga_file(OUT_TGA, img);
    tga_free(img);
}