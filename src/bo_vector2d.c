#include "bo_vector2d.h"

#include <assert.h>
#include <stdio.h>
#include <math.h>

BO_Vector2D BO_Vector2D_create()
{
    return BO_Vector2D_create_xy(0.0f, 0.0f);
}

BO_Vector2D BO_Vector2D_create_xy(float x, float y)
{
    BO_Vector2D v = {.x = x, .y = y};
    return v;
}

BO_Vector2D BO_Vector2D_create_angle_length(float angle, float length)
{
    BO_Vector2D v = {.x = 0.0f, .y = -1.0f};
    angle -= 90.0f;
    angle = angle / 180.0f * 3.14159265358f;

    v.x = length * cos(angle);
    v.y = length * sin(angle);

    return v;
}

void BO_vector2D_add(BO_Vector2D *v, BO_Vector2D *u)
{
    assert(v != NULL);
    assert(u != NULL);

    v->x += u->x;
    v->y += u->y;
}

void BO_vector2D_add_xy(BO_Vector2D *v, float x, float y)
{
    assert(v != NULL);

    v->x += x;
    v->y += y;
}

void BO_vector_show(const BO_Vector2D *v)
{
    assert(v != NULL);

    printf("( %f, %f )\n", v->x, v->y);
}