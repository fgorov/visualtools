#include <iostream>
#include "shapes.h"
#include "display.h"
#include "coords.h"

void draw_line(layer *l, point one, point two)
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
            fill_layer_point(l, pt);
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
            fill_layer_point(l, pt);
        }
    }
}

// fills enclosed shape in layer l
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



// bounding boxes
box get_tri_bounding_box(triangle tri)
{
    box bb;
    bb.tl = (point){std::min(tri.origin.x, std::min(tri.positive.x, tri.negative.x)), std::min(tri.origin.y, std::min(tri.positive.y, tri.negative.y))};
    bb.br = (point){std::max(tri.origin.x, std::max(tri.positive.x, tri.negative.x)), std::max(tri.origin.y, std::max(tri.positive.y, tri.negative.y))};
    return bb;
}

box get_visible_bb(box bb)
{
    bb.tl.x = bb.tl.x < 0.0 ? 0.0 : bb.tl.x > display_bounds.x ? display_bounds.x : bb.tl.x;
    bb.tl.y = bb.tl.y < 0.0 ? 0.0 : bb.tl.y > display_bounds.x ? display_bounds.y : bb.tl.y;
    bb.br.x = bb.br.x < 0.0 ? 0.0 : bb.br.x > display_bounds.x ? display_bounds.x : bb.br.x;
    bb.br.y = bb.br.y < 0.0 ? 0.0 : bb.br.y > display_bounds.y ? display_bounds.y : bb.br.y;
    return bb;
}

point get_bounding_box_size(box bb)
{
    return (point){(bb.br.x - bb.tl.x), (bb.br.y - bb.tl.y)};
}


// triangles
// generates 3 points, not drawn
triangle generate_triangle(point origin, double degrees, double length, double radius)
{
    double radians = degrees * radian;

    triangle tri;
    tri.origin = origin;

    point positive = {origin.x + cos(radians + radius) * length, origin.y + sin(radians + radius) * length};
    tri.positive = positive;   

    point negative = {origin.x + cos(radians - radius) * length, origin.y + sin(radians - radius) * length};
    tri.negative = negative;

    return tri;
}

// connects tri on a layer, presumes has already been drawn to layer. may be better to make it a layer function as opposed to void?
void connect_triangle(layer *l, triangle tri)
{
    fill_layer_point(l, tri.origin);
    fill_layer_point(l, tri.positive);
    fill_layer_point(l, tri.negative);
    draw_line(l, tri.origin, tri.positive);
    draw_line(l, tri.origin, tri.negative);
    draw_line(l, tri.positive, tri.negative);
}

layer draw_triangle(point origin, double degrees, double length, double radius)
{
    layer tri_layer = {};
    triangle tri = generate_triangle(origin, degrees, length, radius);
    connect_triangle(&tri_layer, tri);
    return tri_layer;
}

layer draw_triangle_filled(point origin, double degrees, double length, double radius)
{
    layer tri_layer = draw_triangle(origin, degrees, length, radius);
    fill_primitive(&tri_layer);
    return tri_layer;
}

quad mirror_quad(point origin, point offset)
{
    quad mirrored;
    // plus both
    mirrored.a = add_points(origin, offset);
    // minus x plus y
    mirrored.b = (point){origin.x - offset.x, origin.y + offset.y};
    // mins y plus x
    mirrored.c = (point){origin.x + offset.x, origin.y - offset.y};
    // minus both
    mirrored.d = subtract_points(origin, offset);
/*
    fill_layer_point(&l, (point){x + origin.x, y + origin.y});
    fill_layer_point(&l, (point){-x + origin.x, y + origin.y});
    fill_layer_point(&l, (point){x + origin.x, -y + origin.y});
    fill_layer_point(&l, (point){-x + origin.x, -y + origin.y});
    */
    return mirrored;
}

layer draw_circle(point origin, float r)
{
    layer l = {};
    float P = 1 - r;
    for(pixel px = {r, 0}; px.x > px.y; px.y++)
    {
        fill_layer_quad(&l, mirror_quad(origin, (point){px.x, px.y}));

        P = P <= 0 ? P + (2 * px.y) + 1 : P + (2 * px.y) - (2 * px.x) + 1;
        if( P > 0) px.x--;
        if (px.x < px.y) break;

        if (px.x != px.y)
        {
            fill_layer_quad(&l, mirror_quad(origin, (point){px.y, px.x}));
        }
    }
    return l;
}

layer draw_circle_filled(point origin, float r)
{
    layer l = draw_circle(origin, r);
    fill_primitive(&l);
    return l;
}