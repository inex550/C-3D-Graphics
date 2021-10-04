#ifndef __MODEL_H__
#define __MODEL_H__

typedef struct v3f {
    float x;
    float y;
    float z;
} Vect3f;

typedef struct edge {
    int v;
    int vt;
    int vn;
} Edge;

typedef struct face {
    Edge *edges;
    int nedges;
} Face;

typedef struct model {
    Vect3f *vects;
    Face *faces;

    int nvects;
    int nfaces;
} Model;

Model* read_obj_model(char *filename);

#endif //__MODEL_H__