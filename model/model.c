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
    Array *faces_arr = array_create(sizeof(Face));

    while (fread(type, 2, 1, fptr)) {
        memset(buf, 0, LINE_BUF_LEN);

        if (type[0]=='v'&&type[1]==' ') {
            ptr = buf;
            while ((*ptr++ = fgetc(fptr)) != '\n');

            Vect3f vect;

            vect.x = strtof(buf, &endptr);
            vect.y = strtof(endptr, &endptr);
            vect.z = strtof(endptr, NULL);

            array_add(vects_arr, &vect);
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
    model->nvects = vects_arr->len;

    free(faces_arr);
    free(vects_arr);

    return model;
}

