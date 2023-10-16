#pragma once

/**
 * All possible return codes.
 */
typedef enum BO_Result
{
    // great success
    BO_SUCCESS,

    BO_SDL_INIT_FAILED,

    // for window
    BO_FAILED_TO_ALLOCATE_WINDOW,
    BO_FAILED_TO_CREATE_WINDOW,
    BO_FAILED_TO_GET_SURFACE,
    BO_FAILED_TO_FILL_RECT,
    BO_FAILED_TO_UPDATE_SURFACE,
    BO_FAILED_TO_CREATE_RENDERER,

    // for key mapping
    BO_FAILED_TO_MAP_KEY,
    BO_NO_MORE_EVENTS,

    // for graphics
    BO_FAILED_TO_CLEAR_RENDERER,
    BO_FAILED_TO_SET_RENDER_COLOUR,
    BO_FAILED_TO_DRAW_FILLED_RECT,

    // for lsit
    BO_FAILED_TO_ALLOCATE_LIST,
    BO_FAILED_TO_ALLOCATE_NODE,
    BO_FAILED_TO_ALLOCATE_ITERATOR,
} BO_Result;