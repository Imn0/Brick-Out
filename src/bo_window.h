#ifndef BO_WINDOW_H
#define BO_WINDOW_H

#include "bo_key_events.h"
#include "bo_result.h"

#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct BO_Window
{
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
} BO_Window;

BO_Result BO_Window_create(BO_Window **window);

void BO_Window_destroy(BO_Window *window);

BO_Result BO_Window_get_event(const BO_Window *window, BO_KeyEvent *key_event);

#endif // BO_WINDOW_H
