#include "bo_rectangle.h"

#include <stdio.h>

BO_Rectangle BO_Rectangle_create(const BO_Vector2D *position, float width, float height)
{
    return BO_Rectangle_create_xy(position->x, position->y, width, height);
}

BO_Rectangle BO_Rectangle_create_xy(float x, float y, float width, float height)
{
    BO_Rectangle rectangle = {.position = {.x = x, .y = y}, .height = height, .width = width};
    return rectangle;
}

void BO_Rectangle_translate(BO_Rectangle *rectangle, BO_Vector2D *translation)
{
    BO_vector2D_add(&rectangle->position, translation);
}

void BO_Rectangle_translate_xy(BO_Rectangle *rectangle, float x, float y)
{
    BO_vector2D_add_xy(&rectangle->position, x, y);
}

void BO_Rectangle_set_position(BO_Rectangle *rectangle, BO_Vector2D *position)
{
    rectangle->position = *position;
}

void BO_Rectangle_set_position_xy(BO_Rectangle *rectangle, float x, float y)
{
    rectangle->position.x = x;
    rectangle->position.y = y;
}

void BO_Rectangle_show(BO_Rectangle *rectangle)
{
    printf("rectangle x: %f, y: %f, w: %f, h: %f \n",
           rectangle->position.x,
           rectangle->position.y,
           rectangle->width,
           rectangle->height);
}
