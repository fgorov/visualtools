#include <iostream>
#include <vector>
#include "shapes.h"
#include "display.h"
#include "coords.h"

// general
// write functions
void write_point_to_shape(shape *s, point a)
{
    s->points.push_back(a);
}
template<std::size_t size_unknown>
void write_array_to_shape(shape *s, std::array<point, size_unknown> array)
{
    for(int i = 0; i < array.size(); i++)
        s->points.push_back(array.at(i));
}
void write_shape_to_layer(shape *s, layer *l)
{
    size_t point_count = s->points.size();
    for(int i = 0; i < point_count; i++)
        fill_layer_point(l, s->points.at(i));
}
void write_shape_alpha_to_layer(shape_alpha *s_a, layer *l)
{
    size_t point_count = s_a->points.size();
    for(int i = 0; i < point_count; i++)
        add_layer_point(l, s_a->points.at(i));
}
void write_layer_to_shape(layer *l, shape *s)
{
    for(int y = 0; y < display_bounds.y; y++)
    {
        for(int x = 0; x < display_bounds.x; x++)
        {
            if(l->at(y).at(x) != 0.0)
                s->points.push_back((point){x, y});
        }
    }
}

// conversions

point_alpha point_to_point_alpha(point p)
{
    return (point_alpha){p.x, p.y, 1.0};
}
point point_alpha_to_point(point_alpha p_a)
{
    return (point){p_a.x, p_a.y};
}
shape_alpha shape_to_shape_alpha(shape *s)
{
    shape_alpha s_a;
    size_t s_point_count = s->points.size();
    s_a.origin = s->origin;
    for(int i = 0; i < s_point_count; i++)
        s_a.points.push_back(point_to_point_alpha(s->points.at(i)));
    return s_a;
}

// read functions
void read_shape(shape s)
{
    size_t points = s.points.size();
    for(int i = 0; i < points; i++)
        print_point(s.points.at(i), "shape point");
}

// line / fill functions
void draw_line(shape *s, point one, point two)
{   
    point d = { two.x - one.x , two.y - one.y };
    point absd = {abs(d.x), abs(d.y)};

    //print_coords(d, "d");
    point pt = {one.x, one.y};

    if(absd.x > absd.y) {
        double _d = 2 * absd.y - absd.x;
        for(int i = 0; i < absd.x - 1; i++) {
            pt.x = (d.x < 0) ? d.x - 1 : pt.x + 1;
            if(_d < 0) 
            {
                _d = _d + 2 * absd.y;
            }
            else 
            {
                if(d.y < 0) {pt.y -= 1;}
                else { pt.y += 1;}
                _d = _d + ( 2 * absd.y - 2 * absd.x); 
            }
            s->points.push_back(pt);
        }
    }
    else { 
        double _d = 2 * absd.x - absd.y;

        for(int i = 0; i < absd.y - 1 ; i++)
        {
            pt.y = (d.y < 0) ? pt.y - 1 : pt.y + 1;
            if(_d < 0)
            {
                _d = _d + 2 * absd.x;
            }
            else
            {
                if(d.x < 0)  {pt.x -= 1;}
                else {pt.x += 1;}
                _d = _d + ( 2 * absd.x) - (2 * absd.y);
            }
            s->points.push_back(pt);
        }
    }
}
void fill_primitive(layer *l) 
{
    // for each y, get leftmost and rightmost x value of 1
    // fill in between these.    
    for(int y = 0; y < display_bounds.y; y++) 
    {
        int leftmost_pixel = 0;
        while(l->at(y).at(leftmost_pixel) != 1.0 && leftmost_pixel != display_bounds.x) {
            leftmost_pixel++;
        }

        int rightmost_pixel = display_bounds.x;
        while(l->at(y).at(rightmost_pixel) != 1.0 && rightmost_pixel != 0) {
            rightmost_pixel--;
        }

        for(int x = leftmost_pixel; x < rightmost_pixel; x++)
        {
            l->at(y).at(x) = 1.0;
        }
    }
}
void fill_shape(shape *s)
{
    // write to layer and do it that way :(
    layer buffer = {};
    write_shape_to_layer(s, &buffer);
    fill_primitive(&buffer);
    write_layer_to_shape(&buffer, s);
}

// everything bounding boxes
line get_bounding_box(shape *s)
{
    line bounding_box = {(point){s->points.at(0).x, s->points.at(0).y}, (point){s->points.at(0).x, s->points.at(0).y}};
    size_t vector_size = s->points.size();
    if(vector_size > 1)
    {
        for(int i = 1; i < vector_size; i++)
        {
            point pt = s->points.at(i);
            if(pt.x < bounding_box[0].x) bounding_box[0].x = pt.x;
            if(pt.x > bounding_box[1].x) bounding_box[1].x = pt.x;
            if(pt.y < bounding_box[0].y) bounding_box[0].y = pt.y;
            if(pt.y > bounding_box[1].y) bounding_box[1].y = pt.y;
        }
    }
    return bounding_box;
}
line get_visible_bounding_box(line bb)
{
    bb[0].x = bb[0].x < 0.0 ? 0.0 : bb[0].x > display_bounds.x ? display_bounds.x : bb[0].x;
    bb[0].y = bb[0].y < 0.0 ? 0.0 : bb[0].y > display_bounds.x ? display_bounds.y : bb[0].y;
    bb[1].x = bb[1].x < 0.0 ? 0.0 : bb[1].x > display_bounds.x ? display_bounds.x : bb[1].x;
    bb[1].y = bb[1].y < 0.0 ? 0.0 : bb[1].y > display_bounds.y ? display_bounds.y : bb[1].y;
    return bb;
}
point get_bounding_box_size(line bb)
{
    return (point){(bb[1].x - bb[0].x), (bb[1].y - bb[0].y)};
}

// triangles
void connect_triangle(shape *s)
{
    draw_line(s, s->points.at(0), s->points.at(1));
    draw_line(s, s->points.at(0), s->points.at(2));
    draw_line(s, s->points.at(1), s->points.at(2));
}
shape draw_triangle(point origin, double degrees, double length, double radius)
{
    double radians = degrees * radian;
    shape s;
    s.origin = origin;
    s.points.push_back(origin);
    s.points.push_back((point){origin.x + cos(radians + radius) * length, origin.y + sin(radians + radius) * length});
    s.points.push_back((point){origin.x + cos(radians - radius) * length, origin.y + sin(radians - radius) * length});
    return s;
}
shape draw_triangle_connected(point origin, double degrees, double length, double radius)
{
    shape s = draw_triangle(origin, degrees, length, radius);
    connect_triangle(&s);
    return s;
}
shape draw_triangle_filled(point origin, double degrees, double length, double radius)
{
    shape s = draw_triangle_connected(origin, degrees, length, radius);
    fill_shape(&s);
    return s;
}

// circles
quad mirror_quad(point origin, point offset)
{
    quad mirrored;
    // plus both
    mirrored[0] = add_points(origin, offset);
    // minus x plus y
    mirrored[1] = (point){origin.x - offset.x, origin.y + offset.y};
    // mins y plus x
    mirrored[2]= (point){origin.x + offset.x, origin.y - offset.y};
    // minus both
    mirrored[3] = subtract_points(origin, offset);
    return mirrored;
}
shape draw_circle(point origin, float r)
{
    shape circle = {};
    float P = 1 - r;
    for(pixel px = {r, 0}; px.x > px.y; px.y++)
    {
        write_array_to_shape(&circle, mirror_quad(origin, (point){px.x, px.y}));

        P = P <= 0 ? P + (2 * px.y) + 1 : P + (2 * px.y) - (2 * px.x) + 1;
        if( P > 0) px.x--;
        if (px.x < px.y) break;

        if (px.x != px.y)
        {
            write_array_to_shape(&circle, mirror_quad(origin, (point){px.y, px.x}));
        }
    }
    return circle;
}
shape draw_circle_filled(point origin, float r)
{
    shape circle_filled = draw_circle(origin, r);
    fill_shape(&circle_filled);
    return circle_filled;
}