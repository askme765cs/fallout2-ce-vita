#include "dinput.h"

static int gTouchMouseLastX = 0;
static int gTouchMouseLastY = 0;
static int gTouchMouseDeltaX = 0;
static int gTouchMouseDeltaY = 0;

static int gTouchFingers = 0;
static unsigned int gTouchGestureLastTouchDownTimestamp = 0;
static unsigned int gTouchGestureLastTouchUpTimestamp = 0;
static int gTouchGestureTaps = 0;
static bool gTouchGestureHandled = false;

extern int screenGetWidth();
extern int screenGetHeight();

#ifdef __vita__
#include <psp2/kernel/clib.h>

const uint8_t TOUCH_DELAY = 2;

SDL_GameController* gameController;
float pendingPointerDX;
float pendingPointerDY;
uint8_t numTouches = 0;
uint8_t delayedTouch = 0;
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

    // keep the fractional part for more preciese movement
    pendingPointerDX -= static_cast<int>(pendingPointerDX);
    pendingPointerDY -= static_cast<int>(pendingPointerDY);

    mouseState->buttons[0] = SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) || (numTouches == 1 && delayedTouch == TOUCH_DELAY);
    mouseState->buttons[1] = SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B);

    // Skip 2 frames before triggering LMB event with touchpad. Most UI elements fail to interact on the first frame after the cursor movement
    if (numTouches == 1 && delayedTouch < TOUCH_DELAY)
    {
        delayedTouch++;
    }
#elif __ANDROID__
    mouseState->x = gTouchMouseDeltaX;
    mouseState->y = gTouchMouseDeltaY;
    mouseState->buttons[0] = 0;
    mouseState->buttons[1] = 0;
    gTouchMouseDeltaX = 0;
    gTouchMouseDeltaY = 0;

    if (gTouchFingers == 0) {
        if (SDL_GetTicks() - gTouchGestureLastTouchUpTimestamp > 150) {
            if (!gTouchGestureHandled) {
                if (gTouchGestureTaps == 2) {
                    mouseState->buttons[0] = 1;
                    gTouchGestureHandled = true;
                } else if (gTouchGestureTaps == 3) {
                    mouseState->buttons[1] = 1;
                    gTouchGestureHandled = true;
                }
            }
        }
    } else if (gTouchFingers == 1) {
        if (SDL_GetTicks() - gTouchGestureLastTouchDownTimestamp > 150) {
            if (gTouchGestureTaps == 1) {
                mouseState->buttons[0] = 1;
                gTouchGestureHandled = true;
            } else if (gTouchGestureTaps == 2) {
                mouseState->buttons[1] = 1;
                gTouchGestureHandled = true;
            }
        }
    }
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

void handleTouchFingerEvent(SDL_TouchFingerEvent* event)
{
    int windowWidth = screenGetWidth();
    int windowHeight = screenGetHeight();

    if (event->type == SDL_FINGERDOWN) {
        gTouchFingers++;

        gTouchMouseLastX = (int)(event->x * windowWidth);
        gTouchMouseLastY = (int)(event->y * windowHeight);
        gTouchMouseDeltaX = 0;
        gTouchMouseDeltaY = 0;

        if (event->timestamp - gTouchGestureLastTouchDownTimestamp > 250) {
            gTouchGestureTaps = 0;
            gTouchGestureHandled = false;
        }

        gTouchGestureLastTouchDownTimestamp = event->timestamp;
    } else if (event->type == SDL_FINGERMOTION) {
        int prevX = gTouchMouseLastX;
        int prevY = gTouchMouseLastY;
        gTouchMouseLastX = (int)(event->x * windowWidth);
        gTouchMouseLastY = (int)(event->y * windowHeight);
        gTouchMouseDeltaX += gTouchMouseLastX - prevX;
        gTouchMouseDeltaY += gTouchMouseLastY - prevY;
    } else if (event->type == SDL_FINGERUP) {
        gTouchFingers--;

        int prevX = gTouchMouseLastX;
        int prevY = gTouchMouseLastY;
        gTouchMouseLastX = (int)(event->x * windowWidth);
        gTouchMouseLastY = (int)(event->y * windowHeight);
        gTouchMouseDeltaX += gTouchMouseLastX - prevX;
        gTouchMouseDeltaY += gTouchMouseLastY - prevY;

        gTouchGestureTaps++;
        gTouchGestureLastTouchUpTimestamp = event->timestamp;
    }
}
