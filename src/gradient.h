#ifndef GRADIENT_H
#define GRADIENT_H

#include "coords.h"
#include "display.h"

// general
double gradient_value(point p, point end, greyscale_range range, double angle);

// shapes
void draw_square_gradient(point origin, int *size, greyscale_range range, double angle);
layer draw_triangle_gradient(point origin, double degrees, double length, double radius, greyscale_range range);

#endif