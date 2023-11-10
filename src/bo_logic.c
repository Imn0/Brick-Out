#include "defines.h"
#include "bo_logic.h"
#include "bo_result.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define CHECK_SUCCESS(X, MSG)                   \
    do                                          \
    {                                           \
        BO_Result r = X;                        \
        if (r != BO_SUCCESS)                    \
        {                                       \
            printf("%s [error: %i]\n", MSG, r); \
            exit(1);                            \
        }                                       \
    } while (false)

void BO_init_blocks(BO_List *entities)
{
    BO_create_row(entities, BO_play_boundry_h_top, 0xaa, 0x33, 0x6a);
    BO_create_row(entities, BO_play_boundry_h_top + 30.0f, 0xaa, 0x33, 0x6a);
    BO_create_row(entities, BO_play_boundry_h_top + 60.0f, 0xaa, 0x33, 0x6a);
    BO_create_row(entities, BO_play_boundry_h_top + 90.0f, 0xaa, 0x33, 0x6a);
    BO_create_row(entities, BO_play_boundry_h_top + 120.0f, 0xaa, 0x33, 0x6a);
    BO_create_row(entities, BO_play_boundry_h_top + 150.0f, 0xaa, 0x33, 0x6a);
}

void BO_create_row(BO_List *entities, float y_pos, uint8_t r, uint8_t g, uint8_t b)
{
    float x_pos = 0.0f; // start from the left

    for (int i = 0; i < 8; i++)
    {
        BO_Entity *brick = (BO_Entity *)calloc(1, sizeof(BO_Entity));
        brick->rectangle.position.x = x_pos;
        brick->rectangle.position.y = y_pos;

        brick->rectangle.width = 99.0f;
        brick->rectangle.height = 20.0f;

        brick->r = r;
        brick->g = g;
        brick->b = b;

        CHECK_SUCCESS(BO_List_push_back_dtor(entities, brick, &free), "Brick add fail");

        x_pos += 100.0f;
    }
}

void BO_update_ball(BO_Entity *ball, BO_Vector2D *velocity)
{
    BO_vector2D_add(&ball->rectangle.position, velocity);

    float speed = sqrt(velocity->x * velocity->x + velocity->y * velocity->y);

    // to prevent division by 0
    if (speed < 0.01f)
    {
        speed = 0.1f;
    }

    float factor = BO_ball_desired_speed / speed;
    factor = factor - 1;
    factor /= 100.0f;
    factor += 1;
    velocity->x *= factor;
    velocity->y *= factor;
    if (BO_check_loose(ball))
    {
        BO_reset_ball(ball, velocity);
    }

    if (ball->rectangle.position.y < BO_play_boundry_h_top)
    {
        velocity->y *= -1.0f;
        ball->rectangle.position.y += 5.0f;
    }

    if (ball->rectangle.position.x < 0.0f || ball->rectangle.position.x > 600.0f)
    {
        velocity->x *= -1.0f;
    }
}

void BO_update_paddle(BO_Entity *paddle, BO_Vector2D *velocity)
{

    BO_vector2D_add(&paddle->rectangle.position, velocity);

    if (paddle->rectangle.position.x < 0.0f)
    {
        paddle->rectangle.position.x = 0.0f;
        velocity->x = 0.0f;
    }

    if (paddle->rectangle.position.x + paddle->rectangle.width > 600.0f)
    {
        velocity->x = 0.0f;
        paddle->rectangle.position.x = 600.0f - paddle->rectangle.width;
    }
}

bool BO_check_collision(const BO_Entity *entity1, const BO_Entity *entity2)
{

    return (
        (entity1->rectangle.position.x < entity2->rectangle.position.x + entity2->rectangle.width) &&
        (entity1->rectangle.position.x + entity1->rectangle.width > entity2->rectangle.position.x) &&
        (entity1->rectangle.position.y < entity2->rectangle.position.y + entity2->rectangle.height) &&
        (entity1->rectangle.height + entity1->rectangle.position.y > entity2->rectangle.position.y));
}

void BO_handle_collisions(BO_List *entities, BO_Entity *ball, BO_Vector2D *ball_velocity, const BO_Entity *paddle, const BO_Vector2D *paddle_velocity)
{
    {
        BO_ListItr *itr;
        CHECK_SUCCESS(BO_List_iterator_assign_list(entities, &itr), "itr assign error");

        BO_List_iterator_advance(&itr); // paddle
        BO_List_iterator_advance(&itr); // ball
        BO_List_iterator_advance(&itr); // ui thing

        while (!BO_List_iterator_at_end(itr))
        {
            BO_Entity *brick = (BO_Entity *)BO_List_iterator_value(itr);

            if (BO_check_collision(ball, brick))
            {
                BO_List_remove(entities, itr);
                ball_velocity->y *= -1.0f;

                break;
            }

            BO_List_iterator_advance(&itr);
        }
        BO_List_iterator_destroy(itr);
    }

    if (BO_check_collision(ball, paddle))
    {
        BO_ball_paddle_colision(ball, ball_velocity, paddle, paddle_velocity);
    }
}

void BO_ball_paddle_colision(BO_Entity *ball, BO_Vector2D *ball_velocity, const BO_Entity *paddle, const BO_Vector2D *paddle_velocity)
{
    // calculate distance from center of the paddle
    float distance_from_middle = ball->rectangle.position.x - paddle->rectangle.position.x - paddle->rectangle.width / 2.0f;
    float percentage = abs(distance_from_middle) / (paddle->rectangle.width / 2.0f);

    // calculate angle
    float angle = 50.0f * percentage;
    float direction;

    // keep horizontal direction of the ball the same as before bounce
    if (ball_velocity->x > 0)
        direction = 1.0f;
    else
        direction = -1.0f;
    BO_Vector2D v = BO_Vector2D_create_angle_length(angle, BO_ball_desired_speed);
    v.x = abs(v.x);
    v.x *= direction;

    ball_velocity->x = v.x;
    ball_velocity->y = v.y;

    // ajust ball speed based on paddle movement
    if (paddle_velocity->x > 0.1f || paddle_velocity->x < -0.1f)
    {
        ball_velocity->x += paddle_velocity->x / 3.0f;
    }
}

void BO_reset_ball(BO_Entity *ball, BO_Vector2D *ball_velocity)
{
    ball->rectangle.position.x = 300.0f;
    ball->rectangle.position.y = 400.0f;
    ball_velocity->x = 0.0f;
    ball_velocity->y = -BO_ball_desired_speed;
}

bool BO_check_loose(BO_Entity *ball)
{
    if (ball->rectangle.position.y > BO_paddle_paddle_y + BO_paddle_height)
    {
        return true;
    }
    return false;
}