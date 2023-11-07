#ifndef BO_VECTOR2D_H
#define BO_VECTOR2D_H

typedef struct BO_Vector2D
{
    float x;
    float y;
} BO_Vector2D;

/**
 * Create vector with x and y set to 0.0.
 *
 * @returns
 * vector with x and y set to 0.0.
 */
BO_Vector2D BO_Vector2D_create();

/**
 * Create vector supplied values.
 *
 * @param x
 *   x component.
 *
 * @param y
 *   y component.
 *
 * @returns
 * vector with x and y.
 */
BO_Vector2D BO_Vector2D_create_xy(float x, float y);

/**
 * Create vector supplied values.
 *
 * @param angle
 *   angle, 0 is vertical up, 90 is horizontal right.
 *
 * @param length
 *  length of vector.
 *
 * @returns
 * vector with x and y.
 */
BO_Vector2D BO_Vector2D_create_angle_length(float angle, float length);

/**
 * Add two vectors to each other.
 *
 * @param v
 * Source vector, will be modified.
 *
 * @param u
 * Vector to add to v.
 */
void BO_vector2D_add(BO_Vector2D *v, BO_Vector2D *u);

/**
 * Add values to vector.
 *
 * @param v
 * Source vector, will be modified.
 *
 * @param x
 *   value to add to x component.
 *
 * @param y
 *   value to add to y component.
 *
 */
void BO_vector2D_add_xy(BO_Vector2D *v, float x, float y);

/**
 * print vector to std out
 */
void BO_vector_show(const BO_Vector2D *v);

#endif // BO_VECTOR2D_H