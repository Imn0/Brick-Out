#include "bo_vector2d.h"

#include <assert.h>
#include <stdio.h>

BO_Vector2D BO_Vector2D_create()
{
    return BO_Vector2D_create_xy(0.0f, 0.0f);
}

BO_Vector2D BO_Vector2D_create_xy(float x, float y)
{
    BO_Vector2D v = {.x = x, .y = y};
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