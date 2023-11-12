#ifndef BO_GRAPHICS_H
#define BO_GRAPHICS_H

#include "bo_rectangle.h"
#include "bo_window.h"
#include "bo_logic.h"

#include <stdint.h>

BO_Result BO_Graphics_pre_render(const BO_Window *window);

void BO_Graphics_post_render(const BO_Window *window);

/**
 * Must be called after BO_Graphics_pre_render and before BO_Graphics_post_render
 */
BO_Result BO_Graphics_draw_rectangle(const BO_Window *window, const BO_Rectangle *rectangle, uint8_t r, uint8_t g, uint8_t b);
BO_Result BO_Graphics_display_entity(const BO_Window *window, const BO_Entity *entity, SDL_Rect texture_rect, SDL_Texture *texture);

#endif // BO_GRAPHICS_H