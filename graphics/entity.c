#include "entity.h"
#include <math.h>

#define type_v3_relization(name, minname, type, mintype) \
name minname##_plus_##minname(name a, name b) { \
    return (name){ a.x+b.x, a.y+b.y, a.z+b.z }; \
} \
name minname##_minus_##minname(name a, name b) { \
 return (name){ a.x-b.x, a.y-b.y, a.z-b.z };     \
} \
name minname##_div_##minname(name a, name b) {  \
    return (name){ a.x/b.x, a.y/b.y, a.z/b.z }; \
} \
name minname##_multiply_##minname(name a, name b) { \
    return (name){ a.x*b.x, a.y*b.y, a.z*b.z };     \
} \
name minname##_multiply_##mintype(name a, type minname) { \
    return (name){ a.x*minname, a.y*minname, a.z*minname };\
} \
type mintype##_##minname##_multiply_##minname(name a, name b) { \
    return a.x*b.x + a.y*b.y + a.z*b.z;                         \
} \
name minname##_vect_multiply_##minname(name a, name b) { \
    return (name){ a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x }; \
} \
type minname##_len(name v) { \
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); \
} \
name minname##_normilize(name v) {  \
    type len = minname##_len(v);    \
    return (name){ v.x/len, v.y/len, v.z/len }; \
}

type_v3_relization(Vect3f, v3f, float, f);
type_v3_relization(Vect3i, v3i, int, i);

Vect3i v3i_multiply_f(Vect3i v, float f) {
    return (Vect3i){ (int)(v.x*f), (int)(v.y*f), (int)(v.z*f) };
}

Vect2i v3f_to_v2i(Vect3f v) {
    return (Vect2i){ (int)v.x, (int)v.y };
}

Vect3i v3f_to_v3i(Vect3f v) {
    return (Vect3i){ (int)v.x, (int)v.y, (int)v.z };
}

Vect3f v3i_to_v3f(Vect3i v) {
    return (Vect3f){ (float)v.x, (float)v.y, (float)v.z };
}

Vect2i v2i_plus_v2i(Vect2i a, Vect2i b) {
    return (Vect2i){ a.x + b.x, a.y + b.y };
}

Vect2i v2i_minus_v2i(Vect2i a, Vect2i b) {
    return (Vect2i){ a.x - b.x, a.y - b.y };
}

Vect2i v2i_div_v2i(Vect2i a, Vect2i b) {
    return (Vect2i){ a.x / b.x, a.y / b.y };
}

Vect2i v2i_multiply_v2i(Vect2i a, Vect2i b) {
    return (Vect2i){ a.x * b.x, a.y * b.y };
}

Vect2i v2i_multiply_f(Vect2i a, float b) {
    return (Vect2i){ a.x * b, a.y * b };
}