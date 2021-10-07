#ifndef __MODEL_H__
#define __MODEL_H__

#include "entity.h"

typedef struct {
    Vect3f *vects;
    Vect3f *vtextures;
    Vect3f *vnormals;
    Face *faces;

    int nvects;
    int nvtextures;
    int nvnormals;
    int nfaces;
} Model;

Model* read_obj_model(char *filename);

void model_free(Model *model);

#endif//__MODEL_H__