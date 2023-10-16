#include "bo_window.h"
#include "bo_key_events.h"
#include "bo_result.h"

#include "SDL.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static BO_Result map_sdl_key_to_bo_key(BO_KeyType *bo_type, SDL_KeyCode sdl_type)
{
    switch (sdl_type)
    {
    case SDLK_ESCAPE:
        *bo_type = BO_KEY_QUIT;
        return BO_SUCCESS;

    case SDLK_LEFT:
    case SDLK_a:
        *bo_type = BO_KEY_LEFT;
        return BO_SUCCESS;

    case SDLK_RIGHT:
    case SDLK_d:
        *bo_type = BO_KEY_RIGHT;
        return BO_SUCCESS;
    default:
        return BO_NO_MORE_EVENTS;
    }
}

BO_Result BO_Window_create(BO_Window **window)
{
    BO_Result result = BO_SUCCESS;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        result = BO_SDL_INIT_FAILED;
        goto init_fail;
    }

    BO_Window *new_window = (BO_Window *)calloc(1, sizeof(BO_Window));

    if (new_window == NULL)
    {
        result = BO_FAILED_TO_ALLOCATE_WINDOW;
        goto fail;
    }

    new_window->sdl_window = SDL_CreateWindow("Break Out", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (new_window->sdl_window == NULL)
    {
        result = BO_FAILED_TO_CREATE_WINDOW;
        goto fail;
    }

    new_window->sdl_renderer = SDL_CreateRenderer(new_window->sdl_window, -1, 0);
    if (new_window->sdl_renderer == NULL)
    {
        result = BO_FAILED_TO_CREATE_RENDERER;
        goto fail;
    }

    // assign window
    *window = new_window;
    return result;

fail:
    BO_Window_destroy(new_window);
init_fail:
    return result;
}

void BO_Window_destroy(BO_Window *window)
{
    if (window == NULL)
    {
        return;
    }

    if (window->sdl_renderer != NULL)
    {
        SDL_DestroyRenderer(window->sdl_renderer);
    }
    if (window->sdl_window != NULL)
    {
        SDL_DestroyWindow(window->sdl_window);
    }

    free(window);

    SDL_Quit();

    return;
}

BO_Result BO_Window_get_event(const BO_Window *window, BO_KeyEvent *key_event)
{
    assert(window != NULL);
    assert(key_event != NULL);

    BO_Result result = BO_NO_MORE_EVENTS;

    SDL_Event sdl_event;

    if (SDL_PollEvent(&sdl_event) != 0)
    {
        bool try_map = false;

        if (sdl_event.type == SDL_KEYDOWN)
        {
            key_event->key_state = BO_KEYSTATE_DOWN;
            try_map = true;
        }
        else if (sdl_event.type == SDL_KEYUP)
        {
            key_event->key_state = BO_KEYSTATE_UP;
            try_map = true;
        }

        if (try_map)
        {
            result = map_sdl_key_to_bo_key(&key_event->key_type, sdl_event.key.keysym.sym);
        }
    }

    return result;
}