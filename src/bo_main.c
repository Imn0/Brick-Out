#include "bo_graphics.h"
#include "bo_window.h"
#include "bo_result.h"

#include <stdbool.h>
#include <stdint.h>
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

int main(int argc, char *argv[])
{
    BO_Window *window;
    CHECK_SUCCESS(BO_Window_create(&window), "failed to window");

    bool running = true;
    bool keys_to_process = true;
    BO_KeyEvent key_event;

    while (running)
    {
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
    }

    BO_Window_destroy(window);

    return 0;
}