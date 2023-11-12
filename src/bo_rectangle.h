#ifndef BO_RECTANGLE_H
#define BO_RECTANGLE_H

#include "bo_vector2d.h"

typedef struct BO_Rectangle
{
    BO_Vector2D position;
    BO_Vector2D middle;
    float width;
    float height;
} BO_Rectangle;

BO_Vector2D BO_Rectangle_get_middle(BO_Rectangle *rectangle);

BO_Rectangle BO_Rectangle_create(const BO_Vector2D *position, float width, float height);

BO_Rectangle BO_Rectangle_create_xy(float x, float y, float width, float height);

void BO_Rectangle_translate(BO_Rectangle *rectangle, BO_Vector2D *translation);

void BO_Rectangle_translate_xy(BO_Rectangle *rectangle, float x, float y);

void BO_Rectangle_set_position(BO_Rectangle *rectangle, const BO_Vector2D *position);

void BO_Rectangle_set_position_xy(BO_Rectangle *rectangle, float x, float y);

void BO_Rectangle_show(BO_Rectangle *rectangle);

#endif // BO_RECTANGLE_H