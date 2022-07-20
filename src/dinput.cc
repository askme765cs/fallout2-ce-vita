#include "dinput.h"

#include <SDL.h>

#ifdef __vita__
#include <psp2/kernel/clib.h>

SDL_GameController* gameController;
float pendingPointerDX;
float pendingPointerDY;
int16_t numTouches = 0;
#endif

// 0x4E0400
bool directInputInit()
{
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
        return false;
    }

#ifdef __vita__
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
        return false;
    }
#endif

    if (!mouseDeviceInit()) {
        goto err;
    }

    if (!keyboardDeviceInit()) {
        goto err;
    }

    return true;

err:

    directInputFree();

    return false;
}

// 0x4E0478
void directInputFree()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

// 0x4E04E8
bool mouseDeviceAcquire()
{
    return true;
}

// 0x4E0514
bool mouseDeviceUnacquire()
{
    return true;
}

// 0x4E053C
bool mouseDeviceGetData(MouseData* mouseState)
{
#ifdef __vita__
    mouseState->x = pendingPointerDX;
    mouseState->y = pendingPointerDY;

    pendingPointerDX -= static_cast<int>(pendingPointerDX);
    pendingPointerDY -= static_cast<int>(pendingPointerDY);

    mouseState->buttons[0] = SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) || (numTouches == 1);
    mouseState->buttons[1] = SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B);
#else
    Uint32 buttons = SDL_GetRelativeMouseState(&(mouseState->x), &(mouseState->y));
    mouseState->buttons[0] = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    mouseState->buttons[1] = (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
#endif

    return true;
}

// 0x4E05A8
bool keyboardDeviceAcquire()
{
    return true;
}

// 0x4E05D4
bool keyboardDeviceUnacquire()
{
    return true;
}

// 0x4E05FC
bool keyboardDeviceReset()
{
    SDL_FlushEvents(SDL_KEYDOWN, SDL_TEXTINPUT);
    return true;
}

// 0x4E0650
bool keyboardDeviceGetData(KeyboardData* keyboardData)
{
    return true;
}

// 0x4E070C
bool mouseDeviceInit()
{
#ifdef __vita__
    return true;
#endif
    return SDL_SetRelativeMouseMode(SDL_TRUE) == 0;
}

// 0x4E078C
void mouseDeviceFree()
{
}

// 0x4E07B8
bool keyboardDeviceInit()
{
    return true;
}

// 0x4E0874
void keyboardDeviceFree()
{
}
