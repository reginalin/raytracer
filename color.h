#ifndef COLOR_H
#define COLOR_H
#include "intersection.h"

class Color
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    Color();
    Color(unsigned char r, unsigned char g, unsigned char b);

    void lambert(Intersection intersection);
};

#endif // COLOR_H
