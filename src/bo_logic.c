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

void BO_create_row(BO_List *entities, float y_pos, uint8_t r, uint8_t g, uint8_t b)
{
    float x_pos = 40.0f; // start from the left

    for (int i = 0; i < 7; i++)
    {
        BO_Entity *brick = (BO_Entity *)calloc(1, sizeof(BO_Entity));
        brick->rectangle.position.x = x_pos;
        brick->rectangle.position.y = y_pos;

        brick->rectangle.width = 50.0f;
        brick->rectangle.height = 20.0f;

        brick->r = r;
        brick->g = g;
        brick->b = b;

        CHECK_SUCCESS(BO_List_push_back_dtor(entities, brick, &free), "Brick add fail");

        x_pos += 80.0f;
    }
}

void BO_update_ball(BO_Entity *ball, BO_Vector2D *velocity)
{
    BO_vector2D_add(&ball->rectangle.position, velocity);

    if (ball->rectangle.position.y < 0.0f || ball->rectangle.position.y > 800.0f)
    {
        velocity->y *= -1.0f;
    }

    if (ball->rectangle.position.x < 0.0f || ball->rectangle.position.x > 600.0f)
    {
        velocity->y *= 1.0f;
    }
}

bool BO_check_collision(const BO_Entity *entity1, const BO_Entity *entity2)
{
    // Calculate the coordinates of the left, right, top, and bottom edges of each rectangle.
    // float left1 = entity1->rectangle.position.x;
    // float right1 = entity1->rectangle.position.x + entity1->rectangle.width;
    // float top1 = entity1->rectangle.position.y;
    // float bottom1 = entity1->rectangle.position.y + entity1->rectangle.height;

    // float left2 = entity2->rectangle.position.x;
    // float right2 = entity2->rectangle.position.x + entity2->rectangle.width;
    // float top2 = entity2->rectangle.position.y;
    // float bottom2 = entity2->rectangle.position.y + entity2->rectangle.height;

    // Check for collision by comparing the edges of the rectangles and return the result.
    return (
        (entity1->rectangle.position.x < entity2->rectangle.position.x + entity2->rectangle.width) && // right1 < left2
        (entity1->rectangle.position.x > entity2->rectangle.position.x + entity2->rectangle.width) && // left1 > right2
        (entity1->rectangle.position.y < entity2->rectangle.position.y) &&                            // bottom1 < top2
        (entity1->rectangle.position.y > entity2->rectangle.position.y + entity2->rectangle.height)); // top1 > bottom2
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
        ball_velocity->x *= -1.0f;
        ball_velocity->y *= -1.0f;
    }
}