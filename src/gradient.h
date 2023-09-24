#ifndef GRADIENT_H
#define GRADIENT_H

#include "coords.h"
#include "display.h"

// general
point_alpha gradient_value(point_alpha p, point end, greyscale_range range, double angle);

// shapes
shape_alpha draw_square_gradient(point origin, point end, double angle, greyscale_range range);
shape_alpha draw_triangle_gradient(point origin, double angle, double length, double radius, greyscale_range range);
shape_alpha draw_circle_gradient(point origin, double radius, greyscale_range range);

#endif