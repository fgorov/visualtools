#ifndef DISPLAY_H
#define DISPLAY_H

#define display_width 32
#define display_height 32

#include <array>


// structs
typedef std::array<std::array<double, display_width>, display_height> layer;
extern layer display;
extern std::array<std::array<char, display_width>, display_height> ascii_display;


#include "shapes.h"
#include "coords.h"

extern point display_bounds;

// conversions
char value_to_ascii(double value);

// point fills
void fill_layer_point(layer *l, point pt);
void fill_display_point(point pt);

// shape fills
void fill_layer_quad(layer *l, quad q);

// renders
void render_point_ascii(int x, int y);
void render_display_ascii();

void render_layer(layer l);

// prints
void print_break();
void print_display();
void print_ascii_display();

#endif