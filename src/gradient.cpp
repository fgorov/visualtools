#include <iostream>
#include <math.h>
#include <array>
#include "shapes.h"
#include "gradient.h"
#include "coords.h"
#include "display.h"


// generates single point of gradient
point_alpha gradient_value(point_alpha pt, point size, greyscale_range range, double angle)
{
    // get relative value of point in range and multiply
    // by cos/sin of gradient's angle to get the value of
    // an individual points in the gradient
    double x = (pt.x - size.x) / size.x * cos(radian * angle);
    double y = (pt.y - size.y) / size.y * sin(radian * angle);
    return (point_alpha){pt.x, pt.y, (((x + y) / 2) * (range.end - range.start) + range.start)};
}

point_alpha generate_radial_gradient_value(point origin, point_alpha pt, double radius, greyscale_range range)
{
    // distance between point and origin
    double distance = ((abs(origin.x - pt.x) + abs(origin.y - pt.y)) / 2);
    double brightness = distance / radius;
    return (point_alpha){pt.x, pt.y, brightness * (range.end - range.start) + range.start};
}

// generates gradient over display
shape_alpha draw_square_gradient(point start, point end, double angle, greyscale_range range)
{
    shape_alpha s_a;
    // adjustment is used to normalize the gradient
    // useful when gradient angle > 90 to correct negative values
    double adjustment = 0.0;
    // loop through the gradient's bounding box
    // in the display and fill in the gradient's base values
    // update adjustment with each loop to keep track of the minimum
    for(int y = start.y; y < display_height && y < start.y + end.y; y++) 
    {
        for(int x = start.x; x < display_width && x < start.x + end.x; x++) 
        {
            point_alpha status = gradient_value((point_alpha){x, y, 0.0}, end, range, angle);
            if(status.alpha < adjustment) 
            {
                adjustment = status.alpha;
            }
            s_a.points.push_back(status);
        }
    }
    // if necessary, normalise the gradient with adjustment
    // looping back over the entire gradient is inefficient
    size_t point_count = s_a.points.size();
    if(adjustment < 0)
    {
        for(int i = 0; i < point_count; i++)
        {
            // normalize
            s_a.points.at(i).alpha -= adjustment;
            // clamp to range
            s_a.points.at(i).alpha = s_a.points.at(i).alpha * (range.end - range.start) + range.start;
            // multiply by 0.8 (range.end - range.start) + value, exponential multiplication to increase contrast
            s_a.points.at(i).alpha *= 0.8 + s_a.points.at(i).alpha * M_PI_2;

        }
    }
    return s_a;
}

shape_alpha draw_triangle_gradient(point origin, double angle, double length, double radius, greyscale_range range)
{
    if(radius > 1) {
        radius = 1;
    }
    // make the shapes
    shape t =  draw_triangle(origin, angle, length, radius);
    connect_triangle(&t);
    fill_shape(&t);
    shape_alpha alpha_t = shape_to_shape_alpha(&t);

    // generates a triangle box. finds it bounding box's real size & visible size.
    // it is more efficient to generate the triangle points for this calculation than to run it through the filled shape
    point bb_size = get_bounding_box_size(get_bounding_box(&t));

    // loops over shape and generates a gradient value for each filled point, replacing the value
    size_t point_count = alpha_t.points.size();

    for(int i = 0; i < point_count; i++)
    {
        alpha_t.points.at(i) = gradient_value( alpha_t.points.at(i), bb_size, range, angle); 
    }

    return alpha_t;
}

shape_alpha draw_circle_gradient(point origin, double radius, greyscale_range range)
{
    shape base_shape = draw_circle_filled(origin, radius);
    point s_bounding_box_size = get_bounding_box_size(get_bounding_box(&base_shape));
    size_t point_count = base_shape.points.size();
    shape_alpha alpha_shape = shape_to_shape_alpha(&base_shape);

    for(int i = 0; i < point_count; i++)
    {
        alpha_shape.points.at(i) = generate_radial_gradient_value(origin, alpha_shape.points.at(i), radius, range);
    }

    return alpha_shape;
}