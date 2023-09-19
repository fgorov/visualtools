#ifndef SHAPES_H
#define SHAPES_H

#include "coords.h"

#define radian M_PI / 180   

struct triangle {
    point origin;
    point positive;
    point negative;
};

struct box {
    point tl;
    point br;
};

struct quad {
    point a;
    point b;
    point c;
    point d;
};

#include "display.h"

// general
void draw_line(layer *l, point one, point two);
void fill_primitive(layer *l);

// bounding boxes
point get_bounding_box_size(box bb);
box get_visible_bb(box bb);

// squares
layer draw_square(box square);

// triangles
triangle generate_triangle(point origin, double degrees, double length, double radius);
void connect_triangle(layer *l, triangle tri);
layer draw_triangle(point origin, double degrees, double length, double radius);
layer draw_triangle_filled(point origin, double degrees, double length, double radius);
box get_tri_bounding_box(triangle tri);

// circles
layer draw_circle(point origin, float r);
layer draw_circle_filled(point origin, float r);

#endif