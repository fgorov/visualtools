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
    layer l = {};

    // generates a triangle box. finds it bounding box's real size & visible size.
    shape t = draw_triangle_filled(origin, degrees, length, radius);
    line bb = get_bounding_box(&t);
    point bb_size = get_bounding_box_size(bb);

    // loops over shape and generates a gradient value for each filled point. this should be a function
    size_t t_size = t.points.size();
    for(int i = 0; i < t_size; i++)
    {
        point pt = t.points.at(i);
        l.at(pt.y).at(pt.x) = gradient_value(pt, bb_size, range, degrees);
    }

    return l;
}

double generate_radial_gradient_value(point origin, point pt, double radius, greyscale_range range)
{
    // distance between point and origin
    double distance = ((abs(origin.x - pt.x) + abs(origin.y - pt.y)) / 2);
    double brightness = distance / radius;
    return brightness * (range.end - range.start) + range.start;
}

shape_alpha draw_circle_gradient(point origin, double radius, greyscale_range range)
{
    shape base_shape = draw_circle_filled(origin, radius);
    point s_bounding_box_size = get_bounding_box_size(get_bounding_box(&base_shape));
    size_t point_count = base_shape.points.size();
    shape_alpha alpha_shape = shape_to_shape_alpha(&base_shape);

    for(int i = 0; i < point_count; i++)
    {
        point pt = base_shape.points.at(i);
        alpha_shape.points.at(i).alpha = generate_radial_gradient_value(origin, pt, radius, range);
    }

    return alpha_shape;
}