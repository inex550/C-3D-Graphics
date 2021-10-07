#ifndef __INEX_STD_H__
#define __INEX_STD_H__

#define swap(a, b, type) { \
    type temp = a;  \
    a = b;          \
    b = temp;       \
}

typedef char bool;

#define false 0
#define true 1

#endif//__INEX_STD_H__