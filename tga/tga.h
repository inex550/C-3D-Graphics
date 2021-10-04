#ifndef __TGA_H__
#define __TGA_H__

#define GRAYS 1
#define RGB 3
#define RGBA 4

#pragma pack(push, 1)
struct tga_header {
    char idlen;
    char cmap_type;
    char img_type;

    // Color map description
    short cmap_origin;
    short cmap_len;
    char cmap_el_len;

    // Image description
    short x_origin;
    short y_origin;
    short img_width;
    short img_height;
    char bitsperpixel;
    char img_descriptor;
};

struct tga_image {
    short width;
    short height;

    unsigned char *data;

    char cblen;
};

union tga_color {
    struct {
        unsigned char r, g, b, a;
    };
    unsigned char raw[4];
    unsigned long val;
};
#pragma pack(pop)


typedef struct tga_image TGAImage;
typedef union  tga_color TGAColor;


TGAImage* tga_create(int width, int height, short cblen);

TGAImage* read_tga_file(char *filanme);

int write_tga_file(char *filename, TGAImage *tga_img);

void tga_free(TGAImage *tga_img);

TGAColor tga_get(TGAImage *tga_img, int x, int y);

int tga_set(TGAImage *tga_img, int x, int y, TGAColor color);

TGAColor make_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

#endif //__TGA_H__