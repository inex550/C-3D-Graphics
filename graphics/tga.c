#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inex/std.h"
#include "entity.h"
#include "model.h"
#include "tga.h"

void draw_line(TGAImage *img, Vect2i v0, Vect2i v1, TGAColor color) {

    char trasp = 0;
    if (abs(v0.x - v1.x) < abs(v0.y-v1.y)) {
        swap(v0.x, v0.y, int);
        swap(v1.x, v1.y, int);

        trasp = 1;
    }

    if (v0.x > v1.x) {
        swap(v0.x, v1.x, int);
        swap(v0.y, v1.y, int);
    }

    int dx = (v1.x-v0.x);
    int dy = (v1.y-v0.y);
    int derror = abs(dy)*2;
    int error = 0;
    int y = v0.y; 

    for (int x = v0.x; x <= v1.x; x++) {
        if (trasp)
            tga_set(img, y, x, color);
        else
            tga_set(img, x, y, color);

        error += derror;

        if (error > dx) {
            y += (v1.y>v0.y?1:-1);
            error -= dx*2;
        }
    } 
}

void draw_triangle(TGAImage *img, Vect3i v0, Vect3i v1, Vect3i v2, TGAColor color, int *zbuffer) {
    int width = img->width;

    if (v0.y > v1.y) swap(v0, v1, Vect3i);
    if (v1.y > v2.y) swap(v1, v2, Vect3i);
    if (v0.y > v1.y) swap(v0, v1, Vect3i);

    int total_height = v2.y - v0.y;

    for (int y = v0.y; y < v2.y; y++) {
        bool second_half = y >= v1.y;

        int seg_height = second_half ? v2.y - v1.y : v1.y - v0.y;

        float total_alpha = (float)(y - v0.y) / total_height;
        float seg_alpha = (float)(y - (second_half?v1.y:v0.y)) / seg_height;

        Vect3i start = v3i_plus_v3i(v0, v3i_multiply_f(v3i_minus_v3i(v2, v0), total_alpha));
        Vect3i end = v3i_plus_v3i(second_half?v1:v0, v3i_multiply_f(v3i_minus_v3i(second_half?v2:v1, second_half?v1:v0), seg_alpha));

        if (start.x>end.x) swap(start, end, Vect3i);

        for (int x = start.x; x < end.x; x++) {
            float alpha = (float)(x-start.x) / (end.x-start.x);
            int z = start.z + (float)(end.z-start.z)*alpha;
            int idx = x + y * width;
            if (zbuffer[idx] < z) {
                zbuffer[idx] = z;
                tga_set(img, x, y, color);
            }
        }
    }
}

void draw_polygon(TGAImage *img, Vect3i *vects, int len, TGAColor color, int *zbuffer) {
    if (len == 3) {
        draw_triangle(img, vects[0], vects[1], vects[2], color, zbuffer);
        return;
    }

    for (int ivect = 0; ivect < len; ivect++)
        draw_triangle(img, vects[ivect], vects[(ivect+1)%len], vects[(ivect+2)%len], color, zbuffer);
}

void draw_model_edges(TGAImage *img, char *filename, TGAColor color) {
    int width = img->width;
    int height = img->height;

    Model *model = read_obj_model(filename);

    for (int iface = 0; iface < model->nfaces; iface++) {
        Face face = model->faces[iface];

        for (int i = 0; i < face.nedges; i++) {
            Vect3f v0 = model->vects[face.edges[i].v-1];
            Vect3f v1 = model->vects[face.edges[(i+1)%face.nedges].v-1];

            int x0 = (v0.x+1.)*width/3.;
            int y0 = (v0.y+1.)*height/3.;
            int x1 = (v1.x+1.)*width/3.;
            int y1 = (v1.y+1.)*height/3.;

            draw_line(img, (Vect2i){ x0, y0 }, (Vect2i){ x1, y1 }, color);
        }
    }

    model_free(model);
}

void draw_model_faces(TGAImage *img, char *filename, Vect3f camera_dir, int depth, int *zbuffer) {
    int width = img->width;
    int height = img->height;

    Model *model = read_obj_model(filename);
    if (model == NULL) return;

    for (int iface = 0; iface < model->nfaces; iface++) {
        Face face = model->faces[iface];

        if (face.nedges < 3) continue;

        Vect3i *screen_vects = calloc(sizeof(Vect3i), face.nedges);
        Vect3f *world_vects = calloc(sizeof(Vect3f), face.nedges);

        for (int iedge = 0; iedge < face.nedges; iedge++) {
            Vect3f v = model->vects[face.edges[iedge].v-1];
            screen_vects[iedge] = (Vect3i){ (v.x+2.)*width/4., (v.y+2.)*height/4., v.z*depth };
            world_vects[iedge] = v;
        }

        Vect3f n = v3f_vect_multiply_v3f(v3f_minus_v3f(world_vects[2], world_vects[0]), v3f_minus_v3f(world_vects[1], world_vects[0]));
        n = v3f_normilize(n);

        float intensity = f_v3f_multiply_v3f(n, camera_dir);
        if (intensity>=0) {
            if (intensity>1.) intensity = 1;
            char cnum = intensity*255;
            draw_polygon(img, screen_vects, face.nedges, (TGAColor){ cnum, cnum, cnum, 0 }, zbuffer);
        }
    }

    model_free(model);
}