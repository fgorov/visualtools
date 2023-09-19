#include <iostream>
#include <math.h>
#include <array>
#include "shapes.h"
#include "gradient.h"
#include "coords.h"
#include "display.h"


// generates single point of gradient
double gradient_value(point pt, point size, greyscale_range range, double angle)
{
    // get relative value of point in range and multiply
    // by cos/sin of gradient's angle to get the value of
    // an individual points in the gradient
    double x = pt.x / size.x * cos(radian * angle);
    double y = pt.y / size.y * sin(radian * angle);
    return ((x + y) / 2) * (range.end - range.start) + range.start;
}

// generates gradient over display
layer draw_square_gradient(point origin, point end, greyscale_range range, double angle)
{
    layer l = {};
    // adjustment is used to normalize the gradient
    // useful when gradient angle > 90 to correct negative values
    double adjustment = 0.0;
    // loop through the gradient's bounding box
    // in the display and fill in the gradient's base values
    // update adjustment with each loop to keep track of the minimum
    for(int y = origin.y; y < display_height && y < origin.y + end.y; y++) 
    {
        for(int x = origin.x; x < display_width && x < origin.x + end.x; x++) 
        {
            point pt = {x, y};
            double fill_value = gradient_value(pt, end, range, angle);
            if(fill_value < adjustment) 
            {
                adjustment = fill_value;
            }
            l.at(y).at(x) = fill_value;
        }
    }
    // if necessary, normalise the gradient with adjustment
    // looping back over the entire gradient is inefficient
    if(adjustment < 0)
    {
        for(int y = origin.y; y < display_height && y < origin.y + end.y; y++) 
        {
            for(int x = origin.x; x < display_width && x < origin.x + end.x; x++) 
            {
                l.at(y).at(x) -= adjustment * M_PI_2;
            }
        }
    }
}

layer draw_triangle_gradient(point origin, double degrees, double length, double radius, greyscale_range range)
{
    layer tri_layer = {};

    // generates a triangle box. finds it bounding box's real size & visible size.
    triangle tri = generate_triangle(origin, degrees, length, radius);
    box bb = get_tri_bounding_box(tri);
    point bb_size = abs_point(get_bounding_box_size(bb));
    box visible_bb = get_visible_bb(get_tri_bounding_box(tri));

    connect_triangle(&tri_layer, tri);
    fill_primitive(&tri_layer);

    // loops over shape and generates a gradient value for each filled point. this should be a function
    for(int y = visible_bb.tl.y; y < visible_bb.br.y; y++)
    {
        for(int x = visible_bb.tl.x; x < visible_bb.br.x; x++)
        {
            if(tri_layer.at(y).at(x) == 1.0)
            {
                tri_layer.at(y).at(x) = gradient_value((point){x, y}, bb_size, range, degrees);
            }
        }
    }

    return tri_layer;
}