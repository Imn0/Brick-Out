#include "defines.h"
#include "bo_logic.h"
#include "bo_result.h"

#include <stdlib.h>
#include <stdio.h>

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

void BO_init_ui(BO_List *entities)
{
    BO_Entity *ui = (BO_Entity *)calloc(1, sizeof(BO_Entity));
    ui->rectangle.position.x = 0;
    ui->rectangle.position.y = 0;

    ui->rectangle.width = BO_play_boundry_w;
    ui->rectangle.height = BO_play_boundry_h_top;

    ui->r = 100;
    ui->g = 90;
    ui->b = 240;

    CHECK_SUCCESS(BO_List_push_back_dtor(entities, ui, &free), "Brick add fail");
}

void BO_init_blocks(BO_List *entities)
{
    BO_create_row(entities, BO_play_boundry_h_top, 0xff, 0x00, 0x00);
    BO_create_row(entities, BO_play_boundry_h_top + 30.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, BO_play_boundry_h_top + 60.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, BO_play_boundry_h_top + 90.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, BO_play_boundry_h_top + 120.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, BO_play_boundry_h_top + 150.0f, 0xff, 0x00, 0x00);
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

    if (ball->rectangle.position.y < BO_play_boundry_h_top || ball->rectangle.position.y > BO_play_boundry_h_bot)
    {
        velocity->y *= -1.0f;
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
        velocity->x *= 0.0f;
    }

    if (paddle->rectangle.position.x + paddle->rectangle.width > 600.0f)
    {
        paddle->rectangle.position.x = 0.0f;
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

void BO_handle_collisions(BO_List *entities, BO_Entity *ball, BO_Vector2D *ball_velocity, const BO_Entity *paddle)
{
    {
        BO_ListItr *itr;
        CHECK_SUCCESS(BO_List_iterator_assign_list(entities, &itr), "itr assign error");

        BO_List_iterator_advance(&itr);
        BO_List_iterator_advance(&itr);

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
        // simple very
        ball_velocity->y *= -1.0f;
        ball->rectangle.position.x += 1.0f;
    }
}