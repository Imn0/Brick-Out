#include "bo_graphics.h"
#include "bo_rectangle.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

BO_Result BO_Graphics_pre_render(const BO_Window *window)
{
    BO_Result result = BO_SUCCESS;

    // clear the window to black
    if (SDL_SetRenderDrawColor(window->sdl_renderer, 208, 121, 145, 255) != 0)
    {
        result = BO_FAILED_TO_SET_RENDER_COLOUR;
        goto ret;
    }

    if (SDL_RenderClear(window->sdl_renderer) != 0)
    {
        result = BO_FAILED_TO_CLEAR_RENDERER;
        goto ret;
    }

ret:
    return result;
}

void BO_Graphics_post_render(const BO_Window *window)
{
    SDL_RenderPresent(window->sdl_renderer);
}

BO_Result BO_Graphics_display_entity(const BO_Window *window, const BO_Entity *entity, SDL_Rect texture_rect, SDL_Texture *texture)
{
    BO_Result result = BO_SUCCESS;
    result = BO_Graphics_draw_rectangle(window, &entity->rectangle, entity->r, entity->g, entity->b);
    if (entity->hp == 1)
    {
        SDL_RenderCopy(window->sdl_renderer, texture, NULL, &texture_rect);
    }

    return result;
}

BO_Result BO_Graphics_draw_rectangle(const BO_Window *window, const BO_Rectangle *rectangle, uint8_t r, uint8_t g, uint8_t b)
{
    BO_Result result = BO_SUCCESS;

    // convert internal to SDL rect
    SDL_Rect sdl_rect = {.x = rectangle->position.x, .y = rectangle->position.y, .w = rectangle->width, .h = rectangle->height};

    // set the draw colour
    if (SDL_SetRenderDrawColor(window->sdl_renderer, r, g, b, 0xff) != 0)
    {
        result = BO_FAILED_TO_SET_RENDER_COLOUR;
        goto ret;
    }

    // draw!
    if (SDL_RenderFillRect(window->sdl_renderer, &sdl_rect) != 0)
    {
        result = BO_FAILED_TO_DRAW_FILLED_RECT;
        goto ret;
    }

ret:
    return result;
}