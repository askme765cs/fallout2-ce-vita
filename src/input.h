#ifndef FALLOUT_INPUT_H_
#define FALLOUT_INPUT_H_

#ifdef __vita__
#include <psp2/libime.h>
#include <psp2/kernel/clib.h>
#include <vita2d.h>
#include "dinput.h"
#endif

namespace fallout {

typedef void(IdleFunc)();
typedef void(FocusFunc)(bool focus);
typedef void(TickerProc)();

typedef int(PauseHandler)();
typedef int(ScreenshotHandler)(int width, int height, unsigned char* buffer, unsigned char* palette);

int inputInit(int a1);
void inputExit();
int inputGetInput();
void _process_bk();
void enqueueInputEvent(int a1);
void inputEventQueueReset();
void tickersExecute();
void tickersAdd(TickerProc* fn);
void tickersRemove(TickerProc* fn);
void tickersEnable();
void tickersDisable();
void pauseHandlerConfigure(int keyCode, PauseHandler* fn);
void takeScreenshot();
int screenshotHandlerDefaultImpl(int width, int height, unsigned char* data, unsigned char* palette);
void screenshotHandlerConfigure(int keyCode, ScreenshotHandler* handler);
unsigned int getTicks();
void inputPauseForTocks(unsigned int ms);
void inputBlockForTocks(unsigned int ms);
unsigned int getTicksSince(unsigned int a1);
unsigned int getTicksBetween(unsigned int a1, unsigned int a2);
unsigned int _get_bk_time();
void inputSetKeyboardKeyRepeatRate(int value);
int inputGetKeyboardKeyRepeatRate();
void inputSetKeyboardKeyRepeatDelay(int value);
int inputGetKeyboardKeyRepeatDelay();
void inputSetFocusFunc(FocusFunc* func);
FocusFunc* inputGetFocusFunc();
void inputSetIdleFunc(IdleFunc* func);
IdleFunc* inputGetIdleFunc();
int _GNW95_input_init();
void _GNW95_process_message();
void _GNW95_clear_time_stamps();
void _GNW95_lost_focus();

#ifdef __vita__

extern SDL_GameController* gameController;
extern float gTouchMouseDeltaX;
extern float gTouchMouseDeltaY;
extern uint8_t numTouches;
extern uint8_t delayedTouch;
extern TouchpadMode frontTouchpadMode;
extern TouchpadMode rearTouchpadMode;

void openController();
void closeController();
void handleTouchEventDirect(const SDL_TouchFingerEvent& event);
void processControllerAxisMotion();
void handleControllerAxisEvent(const SDL_ControllerAxisEvent& motion);
void handleControllerButtonEvent(const SDL_ControllerButtonEvent& button);

void vitaActivateIme();
void vitaImeEventHandler(void *arg, const SceImeEventData *e);

void updateVita2dPalette(SDL_Color *colors, int start, int count);
void renderVita2dFrame(SDL_Surface *surface);
void setRenderRect(int width, int height, bool fullscreen);

enum
{
    CONTROLLER_L_DEADZONE = 3000,
    CONTROLLER_R_DEADZONE = 25000,
    VITA_FULLSCREEN_WIDTH = 960,
    VITA_FULLSCREEN_HEIGHT = 544,
    DEFAULT_WIDTH = 640,
    DEFAULT_HEIGHT = 480
};

extern vita2d_texture *texBuffer;
extern uint8_t *palettedTexturePointer;

#endif

} // namespace fallout

#endif /* FALLOUT_INPUT_H_ */
