#include "defines.h"
#include "bo_graphics.h"
#include "bo_window.h"
#include "bo_result.h"
#include "bo_logic.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

/**
 *
 * TODO
 *
 * ADD points rendering
 * ADD frame-rate independent phisics
 * ADD levels
 * ADD normal colours
 * ADD mouse steerign
 * ADD corner bounces
 * ADD ball speed change
 *
 * FIX paddle - ball colisions
 *
 */

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

int main(int argc, char *argv[])
{
    // load font
    TTF_Init();
    TTF_Font *Pixel = TTF_OpenFont("../data/fonts/PixelForce.ttf", 24);
    SDL_Color White = {255, 255, 255};

    // create paddle, -1 hp for infinite health
    BO_Entity paddle = {.rectangle = BO_Rectangle_create_xy(300.0f, BO_paddle_paddle_y, 80.0f, BO_paddle_height), .hp = -1, .r = 0xaa, .g = 0x33, .b = 0x6a};
    BO_Vector2D paddle_velocity = BO_Vector2D_create();

    // create ball, -1 hp for infinite health
    BO_Entity ball = {.rectangle = BO_Rectangle_create_xy(300.0f, 400.0f, 10.0f, 10.0f), .hp = -1, .r = 0x00, .g = 0x00, .b = 0x00};
    BO_Vector2D ball_velocity = BO_Vector2D_create_angle_length(45.0f, BO_ball_desired_speed - 1.0f);

    // create ui element, -1 hp for infinite health
    BO_Entity ui = {.rectangle = BO_Rectangle_create_xy(0.0f, 0.0f, BO_play_boundry_w, BO_play_boundry_h_top), .hp = -1, .r = 0xaa, .g = 0x33, .b = 0x6a};

    BO_Window *window;
    CHECK_SUCCESS(BO_Window_create(&window), "failed to window");

    BO_List *entities = NULL;
    CHECK_SUCCESS(BO_List_assign(&entities), "failed to assign entities list");
    CHECK_SUCCESS(BO_List_push_back(entities, &paddle), "failed to add paddle");
    CHECK_SUCCESS(BO_List_push_back(entities, &ball), "failed to add ball");
    CHECK_SUCCESS(BO_List_push_back(entities, &ui), "failed to add ui element");
    BO_init_blocks(entities);

    // load breaking block texture
    SDL_Surface *tmp_surface = IMG_Load("../data/textures/breaking.png");
    SDL_Texture *breaking_texture = SDL_CreateTextureFromSurface(window->sdl_renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    SDL_Rect texture_rect;
    // width and height of the texture are constant
    texture_rect.w = BO_paddle_width;  // the width of the texture
    texture_rect.h = BO_paddle_height; // the height of the texture

    BO_ListItr *itr = NULL;
    CHECK_SUCCESS(BO_List_iterator_assign_list(entities, &itr), "failed to assign itr");

    bool running = true;
    bool keys_to_process = true;

    BO_KeyEvent key_event;
    uint64_t fps_cap = 100;
    uint64_t framerate_cap = 1000 / fps_cap;
    uint64_t points = 0;
    uint64_t game_start = SDL_GetTicks64();

    while (running)
    {
        uint64_t loop_start = SDL_GetTicks64();
        keys_to_process = true;
        while (keys_to_process)
        {

            BO_Result event_resut = BO_Window_get_event(window, &key_event);
            if (event_resut == BO_SUCCESS)
            {
                switch (key_event.key_type)
                {
                case BO_KEY_QUIT:
                    printf("bye\n");
                    running = false;
                    goto end;
                    break;
                case BO_KEY_LEFT:
                    if (key_event.key_state == BO_KEYSTATE_DOWN)
                    {
                        paddle_velocity.x = -2.0f;
                    }
                    else
                        paddle_velocity.x = 0.0f;
                    break;
                case BO_KEY_RIGHT:
                    if (key_event.key_state == BO_KEYSTATE_DOWN)
                        paddle_velocity.x = 2.0f;
                    else
                        paddle_velocity.x = 0.0f;
                    break;

                default:
                    break;
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

        BO_update_paddle(&paddle, &paddle_velocity);
        BO_update_ball(&ball, &ball_velocity);

        BO_handle_collisions(entities, &ball, &ball_velocity, &paddle, &paddle_velocity);

        // points calculation
        uint64_t new_points = (loop_start - game_start) / 1000;
        if (new_points != points)
        {
            points = new_points;
        }

        BO_List_iterator_reset(entities, &itr);

        BO_Graphics_pre_render(window);

        while (!BO_List_iterator_at_end(itr))
        {
            BO_Entity *entity = (BO_Entity *)BO_List_iterator_value(itr);
            texture_rect.x = entity->rectangle.position.x;
            texture_rect.y = entity->rectangle.position.y;
            BO_Graphics_display_entity(window, entity, texture_rect, breaking_texture);
            // BO_Graphics_draw_rectangle(window, &entity->rectangle, entity->r, entity->g, entity->b);

            BO_List_iterator_advance(&itr);
        }

        // points rendering
        char str[256];
        sprintf(str, "%lu", points);
        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Pixel, str, White);
        SDL_Rect Message_rect;                          // create a rect
        Message_rect.x = 300;                           // controls the rect's x coordinate
        Message_rect.y = 0;                             // controls the rect's y coordinte
        Message_rect.w = 33 * ((int)log10(points) + 1); // controls the width of the rect
        Message_rect.h = 48;                            // controls the height of the rect
        SDL_Texture *Message = SDL_CreateTextureFromSurface(window->sdl_renderer, surfaceMessage);
        SDL_FreeSurface(surfaceMessage);
        SDL_RenderCopy(window->sdl_renderer, Message, NULL, &Message_rect);

        BO_Graphics_post_render(window);

        uint64_t delta = SDL_GetTicks() - loop_start;
        if (delta < framerate_cap)
        {
            SDL_Delay(framerate_cap - delta);
        }
    }
end:
    BO_Window_destroy(window);
    return 0;
}
