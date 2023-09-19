#include <iostream>
#include <string>
#include "display.h"
#include "coords.h"
#include "math.h"

// general
point lerp(point start, point end, double i) 
{
    return (point){start.x + (end.x - start.x) * i, start.y + (end.y - start.y) * i};
}

// points
void print_point(point pt, std::string name)
{
    std::cout << name << ": " << pt.x << ", " << pt.y << std::endl;
}

point add_points(point a, point b)
{
    return (point){a.x + b.x, a.y + b.y};
}

point subtract_points(point a, point b)
{
    return (point){a.x - b.x, a.y - b.y};
}

point mirror_point(point origin, point p)
{
    return subtract_points(origin, p);
}

point ceil_point(point pt)
{
    pt.x = ceil(pt.x);
    pt.y = ceil(pt.y);
    return pt;
}

point abs_point(point pt)
{
    return (point){abs(pt.x), abs(pt.y)};
}

point clamp_point(point pt) {
    // this looks scary, but its just two ternary operations
    // the first one checks if its less than 0, and if so sets it to 0
    // if it is more than 0, the second one checks if its higher than the display boundary
    // if it is, it is set to the display boundary, if it isn't, it is unchanged
    pt.x = pt.x < 0.0 ? 0.0 : pt.x > display_bounds.x ? display_bounds.x : pt.x;
    pt.y = pt.y < 0.0 ? 0.0 : pt.y > display_bounds.y ? display_bounds.y : pt.y;
    return pt;
}

pixel point_to_pixel(point pt) 
{
    pixel px = {round(pt.x), round(pt.y)};
    return px;
}

bool is_oob(point pt)
{
    return (pt.x < 0 || pt.x >= display_width) || (pt.y < 0 || pt.y >= display_width);
}



// pixels
void print_pixel(pixel pt, std::string name)
{
    std::cout << name << ": " << pt.x << ", " << pt.y << std::endl;
}

pixel ceil_point_to_pixel(point pt)
{
    return point_to_pixel(ceil_point(pt));
}



// bool2Ds
void print_bool2D(bool2D b, std::string name)
{
    std::cout << name << ": " << b.x << ", " << b.y << std::endl;
}