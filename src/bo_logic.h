#include "bo_rectangle.h"
#include "bo_result.h"
#include "bo_list.h"
#include "bo_vector2d.h"

#include <stdint.h>

typedef struct BO_Entity
{
    BO_Rectangle rectangle;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} BO_Entity;

void BO_init_blocks(BO_List *entities);

void BO_create_row(BO_List *entities, float y_pos, uint8_t r, uint8_t g, uint8_t b);

void BO_update_ball(BO_Entity *ball, BO_Vector2D *velocity);

void BO_update_paddle(BO_Entity *paddle, BO_Vector2D *velocity);

void BO_ball_paddle_colision(const BO_Entity *ball, BO_Vector2D *ball_velocity, const BO_Entity *paddle);

bool BO_check_collision(const BO_Entity *entity1, const BO_Entity *entity2);

void BO_handle_collisions(BO_List *entities, BO_Entity *ball, BO_Vector2D *ball_velocity, const BO_Entity *paddle);