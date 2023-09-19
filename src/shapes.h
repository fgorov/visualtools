#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
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
typedef std::array<point, 2> line;
typedef std::array<point, 4> quad;

struct point_alpha
{
    double x;
    double y;
    double alpha;
};

struct shape {
    point origin;
    std::vector<point> points;
};
struct shape_alpha {
    point origin;
    std::vector<point_alpha> points;
};

#include "display.h"

// general
// write functions
void write_point_to_shape(shape *s, point a);
template<std::size_t size_unknown>
void write_array_to_shape(shape *s, std::array<point, size_unknown> array);
void write_shape_to_layer(shape *s, layer *l);
void write_shape_alpha_to_layer(shape_alpha *s_a, layer *l);
void write_layer_to_shape(layer *l, shape *s);

// conversions
shape_alpha shape_to_shape_alpha(shape *s);

// read functions
void read_shape(shape s);

// lines / fill functions
void draw_line(shape *s, point one, point two);
void fill_primitive(layer *l);
void fill_shape(shape *s);

// everything bounding boxes
line get_bounding_box(shape *s);
line get_visible_bounding_box(line bb);
point get_bounding_box_size(line bb);


// everything triangles
shape draw_triangle(point origin, double degrees, double length, double radius);
void connect_triangle(shape *s);
shape draw_triangle_connected(point origin, double degrees, double length, double radius);
shape draw_triangle_filled(point origin, double degrees, double length, double radius);

// everything squares
layer draw_square(box square);

// everything circles
shape draw_circle(point origin, float r);
shape draw_circle_filled(point origin, float r);

#endif