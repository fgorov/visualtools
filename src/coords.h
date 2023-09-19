#ifndef COORDS_H
#define COORDS_H

#include <string>
#include <stdbool.h>

struct point {
    double x;
    double y;
};

struct pixel {
    int x;
    int y;
};

struct bool2D {
    bool x;
    bool y;
};

struct greyscale_range {
    float start;
    float end;
};

// general
point lerp(point start, point end, double i);

// points
void print_point(point p, std::string name);
point add_points(point a, point b);
point subtract_points(point a, point b);
point mirror_point(point origin, point p);
point ceil_point(point p);
point abs_point(point pt);
point clamp_point(point p);
bool is_oob(point p);

// pixels
void print_pixel(pixel p, std::string name);
pixel point_to_pixel(point p);
pixel ceil_point_to_pixel(point p);

// bool2D
void print_bool2D(bool2D b, std::string name);


#endif