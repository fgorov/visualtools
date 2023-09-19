#include <iostream>
#include "display.h"
#include "gradient.h"
#include "coords.h"
#include "shapes.h"

int main(int argc, char** argv)
{
    greyscale_range range = {0.2, 1.0};

    print_break();

    int x;
    point origin;
    std::cout << "Origin X: ";
    std::cin >> origin.x;

    int y;
    std::cout << "Origin Y: ";
    std::cin >> origin.y;

    /*double angle;
    std::cout << "Angle: ";
    std::cin >> angle;
    
    double length;
    std::cout << "Length: ";
    std::cin >> length;*/

    float radius;
    std::cout << "Radius: ";
    std::cin >> radius;

    //layer l = generate_filled_triangle(origin, angle, length, radius);
    //layer l = generate_gradient_triangle(origin, angle, length, radius, range);

    layer l = draw_filled_circle(origin, radius);

    print_break();

    render_layer(l);
    print_display();

    print_break();

    render_display_ascii();
    print_ascii_display();

    return 0;
}