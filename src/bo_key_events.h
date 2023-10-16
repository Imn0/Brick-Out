#pragma once

/**
 * Possible key states.
 */
typedef enum BO_KeyState
{
    BO_KEYSTATE_UP,
    BO_KEYSTATE_DOWN
} BO_KeyState;

/**
 * Avaiable keys.
 */
typedef enum BO_KeyType
{
    BO_KEY_LEFT,
    BO_KEY_RIGHT,
    BO_KEY_QUIT
} BO_KeyType;

/**
 * Key press event struct.
 */
typedef struct BO_KeyEvent
{
    BO_KeyType key_type;
    BO_KeyState key_state;
} BO_KeyEvent;
