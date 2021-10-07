#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "../inex/array.h"

#define LINE_BUF_LEN 128

Model* read_obj_model(char *filename) {

    Model *model = malloc(sizeof(Model));
    if (model == NULL) return NULL;
    memset(model, 0, sizeof model);

    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL) return NULL;

    char type[2], buf[LINE_BUF_LEN];
    char *ptr, *endptr;

    Array *vects_arr = array_create(sizeof(Vect3f));
    Array *vtextures_arr = array_create(sizeof(Vect3f));
    Array *vnormals_arr = array_create(sizeof(Vect3f));
    Array *faces_arr = array_create(sizeof(Face));

    while (fread(type, 2, 1, fptr)) {
        memset(buf, 0, LINE_BUF_LEN);

        if (type[0]=='v') {
            ptr = buf;
            while ((*ptr++ = fgetc(fptr)) != '\n');

            if (type[1] != ' ') fgetc(fptr);

            Vect3f vect;

            vect.x = strtof(buf, &endptr);
            vect.y = strtof(endptr, &endptr);
            vect.z = strtof(endptr, NULL);

            switch(type[1]) {
                case ' ':
                    array_add(vects_arr, &vect);
                    break;
                case 't':
                    array_add(vtextures_arr, &vect);
                    break;
                case 'n':
                    array_add(vnormals_arr, &vect);
                    break;
            }
        } else if (*type == 'f') {
            ptr = buf;
            while ((*ptr++ = fgetc(fptr)) != '\n');

            Array *edges_arr = array_create(sizeof(Edge));

            endptr = buf;

            while (*endptr != '\r' && *endptr != '\n') {
                Edge edge;
                edge.v = strtol(endptr, &endptr, 10);
                endptr++;
                edge.vt = strtol(endptr, &endptr, 10);
                endptr++;
                edge.vn = strtol(endptr, &endptr, 10);
                array_add(edges_arr, &edge);
            }

            Face face;
            face.edges = (Edge*)edges_arr->ptr;
            face.nedges = edges_arr->len;
            free(edges_arr);

            array_add(faces_arr, &face);
        } else 
            while (fgetc(fptr) != '\n');
    }

    model->faces = (Face*)faces_arr->ptr;
    model->nfaces = faces_arr->len;

    model->vects = (Vect3f*)vects_arr->ptr;
    model->vtextures = (Vect3f*)vtextures_arr->ptr;
    model->vnormals = (Vect3f*)vnormals_arr->ptr;

    model->nvects = vects_arr->len;
    model->nvtextures = vtextures_arr->len;
    model->nvnormals = vnormals_arr->len;

    free(vects_arr);
    free(vtextures_arr);
    free(vnormals_arr);
    free(faces_arr);

    return model;
}

void model_free(Model *model) {
    free(model->vects);
    free(model->vtextures);
    free(model->vnormals);
    free(model->faces);
    free(model);
}