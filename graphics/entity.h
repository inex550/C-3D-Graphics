#ifndef __GRAPHICS_ENTITY_H__
#define __GRAPHICS_ENTITY_H__

#define type_v3(name, minname, type, mintype) \
typedef struct {\
    type x;     \
    type y;     \
    type z;     \
} name;         \
name minname##_plus_##minname(name a, name b);                  \
name minname##_minus_##minname(name a, name b);                 \
name minname##_div_##minname(name a, name b);                   \
name minname##_multiply_##minname(name a, name b);              \
name minname##_multiply_##mintype(name a, type b);              \
type mintype##_##minname##_multiply_##minname(name a, name b);  \
name minname##_vect_multiply_##minname(name a, name b);         \
type minname##_len(name v);                                     \
name minname##_normilize(name v);

typedef struct {
    int x;
    int y;
} Vect2i;

typedef struct {
    int v;
    int vt;
    int vn;
} Edge;

typedef struct {
    Edge *edges;
    int nedges;
} Face;

type_v3(Vect3f, v3f, float, f);
type_v3(Vect3i, v3i, int, i);

Vect3i v3i_multiply_f(Vect3i v, float f);

Vect2i v3f_to_v2i(Vect3f v);
Vect3i v3f_to_v3i(Vect3f v);
Vect3f v3i_to_v3f(Vect3i v);

Vect2i v2i_plus_v2i(Vect2i a, Vect2i b);
Vect2i v2i_minus_v2i(Vect2i a, Vect2i b);
Vect2i v2i_div_v2i(Vect2i a, Vect2i b);
Vect2i v2i_multiply_v2i(Vect2i a, Vect2i b);

Vect2i v2i_multiply_f(Vect2i a, float b);

#endif//__GRAPHICS_ENTITY_H__