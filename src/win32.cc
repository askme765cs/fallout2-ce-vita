#include "win32.h"

#include "core.h"
#include "main.h"
#include "window_manager.h"

#include <SDL.h>
#include <stdlib.h>

#ifdef __vita__
#include <psp2/power.h>
#include <psp2/sysmodule.h>

int _newlib_heap_size_user = 224 * 1024 * 1024;
#endif

#ifdef _WIN32
// 0x51E444
bool gProgramIsActive = false;

// GNW95MUTEX
HANDLE _GNW95_mutex = NULL;

// 0x4DE700
int main(int argc, char* argv[])
{
    _GNW95_mutex = CreateMutexA(0, TRUE, "GNW95MUTEX");
    if (GetLastError() == ERROR_SUCCESS) {
        SDL_ShowCursor(SDL_DISABLE);

        gProgramIsActive = true;
        falloutMain(argc, argv);

        CloseHandle(_GNW95_mutex);
    }
    return 0;
}
#else
bool gProgramIsActive = false;

int main(int argc, char* argv[])
{
#if __APPLE__
    char* basePath = SDL_GetBasePath();
    chdir(basePath);
    SDL_free(basePath);
#endif

#if __ANDROID__
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    chdir(SDL_AndroidGetExternalStoragePath());
#endif

    SDL_ShowCursor(SDL_DISABLE);
    gProgramIsActive = true;

#ifdef __vita__
    chdir("ux0:data/fallout2/");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    sceSysmoduleLoadModule(SCE_SYSMODULE_IME);
	scePowerSetArmClockFrequency(444);
    scePowerSetGpuClockFrequency(222);
    scePowerSetBusClockFrequency(222);
	scePowerSetGpuXbarClockFrequency(166);
#endif

    return falloutMain(argc, argv);
}
#endif
