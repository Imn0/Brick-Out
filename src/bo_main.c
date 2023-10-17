#include "bo_graphics.h"
#include "bo_window.h"
#include "bo_result.h"
#include "bo_logic.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "SDL.h"

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

void test()
{
    printf("start\n");
    BO_Entity paddle = {.rectangle = BO_Rectangle_create_xy(300.0f, 750.0f, 80.0f, 20.0f), .r = 0xff, .g = 0x00, .b = 0x00};
    BO_Vector2D paddle_velocity = BO_Vector2D_create();
    printf("end\n");
}

int main(int argc, char *argv[])
{
    // test();
    BO_Entity paddle = {.rectangle = BO_Rectangle_create_xy(300.0f, 750.0f, 80.0f, 20.0f), .r = 0xff, .g = 0x00, .b = 0x00};
    BO_Vector2D paddle_velocity = BO_Vector2D_create();

    BO_Entity ball = {.rectangle = BO_Rectangle_create_xy(300.0f, 400.0f, 10.0f, 10.0f), .r = 0xff, .g = 0xff, .b = 0xff};
    BO_Vector2D ball_velocity = BO_Vector2D_create_xy(0.0f, 0.5f);

    BO_List *entities = NULL;
    CHECK_SUCCESS(BO_List_assign(&entities), "failed to assign entities list");

    CHECK_SUCCESS(BO_List_push_back(entities, &paddle), "failed to add paddle");
    CHECK_SUCCESS(BO_List_push_back(entities, &ball), "failed to add ball");

    BO_create_row(entities, 30.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, 60.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, 90.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, 120.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, 150.0f, 0xff, 0x00, 0x00);
    BO_create_row(entities, 180.0f, 0xff, 0x00, 0x00);

    BO_ListItr *itr = NULL;
    CHECK_SUCCESS(BO_List_iterator_assign_list(entities, &itr), "failed to assign itr");

    BO_Window *window;
    CHECK_SUCCESS(BO_Window_create(&window), "failed to window");

    bool running = true;
    bool keys_to_process = true;

    BO_KeyEvent key_event;
    uint64_t fps_cap = 60;
    uint64_t framerate_cap = 1000 / 60;

    while (running)
    {
        int loop_start = SDL_GetTicks64();
        keys_to_process = true;
        while (keys_to_process)
        {
            BO_Result event_resut = BO_Window_get_event(window, &key_event);
            if (event_resut == BO_SUCCESS)
            {
                if (key_event.key_type == BO_KEY_QUIT)
                {
                    running = false;
                }
            }
            else if (event_resut == BO_NO_MORE_EVENTS)
            {
                keys_to_process = false;
            }
            else
            {
                printf("error getting event\n");
                exit(1);
            }
        }

        // padle here
        BO_update_ball(&ball, &ball_velocity);
        BO_handle_collisions(entities, &ball, &ball_velocity, &paddle);

        BO_List_iterator_reset(entities, &itr);

        BO_Graphics_pre_render(window);

        while (!BO_List_iterator_at_end(itr))
        {
            BO_Entity *entity = (BO_Entity *)BO_List_iterator_value(itr);
            BO_Graphics_draw_rectangle(window, &entity->rectangle, entity->r, entity->g, entity->b);

            BO_List_iterator_advance(&itr);
        }

        BO_Graphics_post_render(window);

        uint64_t delta = SDL_GetTicks() - loop_start;
        if (delta < framerate_cap)
        {
            SDL_Delay(framerate_cap - delta);
        }
    }

    BO_Window_destroy(window);
    return 0;
}