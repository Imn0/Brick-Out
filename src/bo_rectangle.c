#include "bo_rectangle.h"

#include <stdio.h>

BO_Vector2D BO_Rectangle_get_middle(BO_Rectangle *rectangle)
{
    BO_Vector2D middle = {.x = rectangle->position.x + rectangle->width / 2.0f,
                          .y = rectangle->position.y + rectangle->height / 2.0f};
    return middle;
}

BO_Rectangle BO_Rectangle_create(const BO_Vector2D *position, float width, float height)
{
    BO_Rectangle rectangle = {.position = *position, .height = height, .width = width};
    rectangle.middle = BO_Rectangle_get_middle(&rectangle);
    return rectangle;
}

BO_Rectangle BO_Rectangle_create_xy(float x, float y, float width, float height)
{
    BO_Rectangle rectangle = {.position = {.x = x, .y = y}, .height = height, .width = width};
    rectangle.middle = BO_Rectangle_get_middle(&rectangle);
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

void BO_Rectangle_set_position(BO_Rectangle *rectangle, const BO_Vector2D *position)
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
