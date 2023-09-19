#include "display.h"
#include "shapes.h"
#include <iostream>
#include <math.h>
#include <string.h>
#include "coords.h"

layer display = {};
std::array<std::array<char, display_width>, display_height> ascii_display = {' '};
point display_bounds = {display_width - 1, display_height - 1};


char value_to_ascii(double value)
{
    int index = floor(value * 7);
    if(index > 7) index = 7;
    if(index < 0) index = 0;
    return " -=+*#%@"[index];
}

void render_point_ascii(int x, int y) {
    ascii_display.at(y).at(x) = value_to_ascii(display[y][x]);
}

void render_display_ascii() {
    for(int y = 0; y < display_height; y++) {
        for(int x = 0; x < display_width; x++) {
            render_point_ascii(x, y);
        }
    }
}

void fill_display_point(point pt)
{
    display[(int)pt.y][(int)pt.x] = 1.0;
}

void fill_layer_point(layer *l, point pt)
{
    pixel px = point_to_pixel(clamp_point(pt));
    l->at(px.y).at(px.x) = 1.0;
}

void fill_layer_quad(layer *l, quad q)
{
    fill_layer_point(l, q.a);
    fill_layer_point(l, q.b);
    fill_layer_point(l, q.c);
    fill_layer_point(l, q.d);
}

void render_layer(layer l)
{
    for(int y = 0; y < display_bounds.y; y++)
    {
        for(int x = 0; x < display_bounds.x; x++)
        {
            display[y][x] += l[y][x];
        }
    }
}

void print_break()
{
    for(int i = 0; i < display_bounds.x; i++) std::cout << " - ";
    std::cout << std::endl;
}

void print_display() 
{
    for(int y = 0; y < display_height; y++) {
        for(int x = 0; x < display_width; x++) {
            std::cout << display[y][x] << " ";
        }
        std::cout << std::endl;
    }
}

void print_ascii_display() 
{
    for(int y = 0; y < display_height; y++) {
        for(int x = 0; x < display_width; x++) {
            std::cout << ascii_display.at(y).at(x);
        }
        std::cout << std::endl;
    }
}