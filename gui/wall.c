#include "wall.h"


void initialize_wall (Wall *wall, Color *color, float topLeftX, float topLeftY, 
                      float width, float height) {
    wall->color.red = color->red;
    wall->color.green = color->green;
    wall->color.blue = color->blue;
    wall->color.alpha = color->alpha;
    wall->shape.x = topLeftX;
    wall->shape.y = topLeftY;
    wall->shape.w = width;
    wall->shape.h = height;
}