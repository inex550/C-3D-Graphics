#ifndef __GRAPHICS_TGA_H__
#define __GRAPHICS_TGA_H__

#include "../tga/tga.h"
#include "model.h"

void draw_line(TGAImage *img, Vect2i v0, Vect2i v1, TGAColor color);

void draw_triangle(TGAImage *img, Vect3i v0, Vect3i v1, Vect3i v2, TGAColor color, int *zbuffer);

void draw_polygon(TGAImage *img, Vect3i *vects, int len, TGAColor color, int *zbuffer);

void draw_model_edges(TGAImage *img, char *filename, TGAColor color);

void draw_model_faces(TGAImage *img, char *filename, Vect3f camera_dir, int depth, int *zbuffer);

#endif//__GRAPHICS_TGA_H__