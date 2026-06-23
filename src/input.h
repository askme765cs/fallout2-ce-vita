#ifndef FALLOUT_INPUT_H_
#define FALLOUT_INPUT_H_

#ifdef __vita__
#include <SDL.h>
#include <psp2/libime.h>
#include <psp2/kernel/clib.h>
#include "dinput.h"
#endif

namespace fallout {

typedef void(TickerProc)();

typedef int(ScreenshotHandler)(int width, int height, unsigned char* buffer, unsigned char* palette);

// global for blocking mouse event in inventoryOpenUseItemOn inventory screen
extern bool gBlockMouseUpEvent;

int inputInit();
void inputExit();
int inputGetInput();
void get_input_position(int* x, int* y);
void _process_bk();
void enqueueInputEvent(int logicalKey);
void inputEventQueueReset();
void tickersExecute();
void tickersAdd(TickerProc* fn);
void tickersRemove(TickerProc* fn);
void tickersEnable();
void tickersDisable();
void takeScreenshot();
int screenshotHandlerDefaultImpl(int width, int height, unsigned char* data, unsigned char* palette);
int screenshotHandlerPngImpl(int width, int height, unsigned char* data, unsigned char* palette);
void screenshotHandlerConfigure(int keyCode, ScreenshotHandler* handler);
unsigned int getTicks();
void inputPauseForTocks(unsigned int ms);
void inputBlockForTocks(unsigned int ms);
unsigned int getTicksSince(unsigned int start);
unsigned int getTicksBetween(unsigned int end, unsigned int start);
unsigned int _get_bk_time();
int _GNW95_input_init();
void _GNW95_process_message();
void _GNW95_clear_time_stamps();
void _GNW95_lost_focus();

void beginTextInput();
void endTextInput();

#ifdef __vita__
extern SDL_GameController* gameController;

void openController();
void closeController();
void processControllerAxisMotion();
void handleControllerAxisEvent(const SDL_ControllerAxisEvent& motion);
void handleControllerButtonEvent(const SDL_ControllerButtonEvent& button);

void vitaActivateIme();
void vitaImeEventHandler(void* arg, const SceImeEventData* e);
#endif

} // namespace fallout

#endif /* FALLOUT_INPUT_H_ */
