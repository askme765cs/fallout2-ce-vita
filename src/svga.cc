#include "svga.h"

#include <limits.h>
#include <string.h>

#include <SDL.h>

#include "config.h"
#include "draw.h"
#include "interface.h"
#include "memory.h"
#include "mmx.h"
#include "mouse.h"
#include "win32.h"
#include "window_manager.h"
#include "window_manager_private.h"

namespace fallout {

#ifdef __vita__
#include <vita2d.h>

vita2d_texture *texBuffer;
uint8_t *palettedTexturePointer;
SDL_Rect renderRect;
SDL_Surface *vitaPaletteSurface = NULL;
#endif

static bool createRenderer(int width, int height);
static void destroyRenderer();

// 0x51E2C8
bool gMmxEnabled = true;

// screen rect
Rect _scr_size;

// 0x6ACA18
void (*_scr_blit)(unsigned char* src, int src_pitch, int a3, int src_x, int src_y, int src_width, int src_height, int dest_x, int dest_y) = _GNW95_ShowRect;

// 0x6ACA1C
void (*_zero_mem)() = NULL;

SDL_Window* gSdlWindow = NULL;
SDL_Surface* gSdlSurface = NULL;
SDL_Renderer* gSdlRenderer = NULL;
SDL_Texture* gSdlTexture = NULL;
SDL_Surface* gSdlTextureSurface = NULL;

// TODO: Remove once migration to update-render cycle is completed.
FpsLimiter sharedFpsLimiter;

// 0x4CACD0
void mmxSetEnabled(bool a1)
{
    // 0x51E2CC
    static bool probed = false;

    // 0x6ACA20
    static bool supported;

    if (!probed) {
        supported = mmxIsSupported();
        probed = true;
    }

    if (supported) {
        gMmxEnabled = a1;
    }
}

// 0x4CAD08
int _init_mode_320_200()
{
    return _GNW95_init_mode_ex(320, 200, 8);
}

// 0x4CAD40
int _init_mode_320_400()
{
    return _GNW95_init_mode_ex(320, 400, 8);
}

// 0x4CAD5C
int _init_mode_640_480_16()
{
    return -1;
}

// 0x4CAD64
int _init_mode_640_480()
{
    return _init_vesa_mode(640, 480);
}

// 0x4CAD94
int _init_mode_640_400()
{
    return _init_vesa_mode(640, 400);
}

// 0x4CADA8
int _init_mode_800_600()
{
    return _init_vesa_mode(800, 600);
}

// 0x4CADBC
int _init_mode_1024_768()
{
    return _init_vesa_mode(1024, 768);
}

// 0x4CADD0
int _init_mode_1280_1024()
{
    return _init_vesa_mode(1280, 1024);
}

// 0x4CADF8
void _get_start_mode_()
{
}

// 0x4CADFC
void _zero_vid_mem()
{
    if (_zero_mem) {
        _zero_mem();
    }
}

// 0x4CAE1C
int _GNW95_init_mode_ex(int width, int height, int bpp)
{
    bool fullscreen = true;

    Config resolutionConfig;
    if (configInit(&resolutionConfig)) {
        if (configRead(&resolutionConfig, "f2_res.ini", false)) {
            int screenWidth;
            if (configGetInt(&resolutionConfig, "MAIN", "SCR_WIDTH", &screenWidth)) {
                width = screenWidth;
            }
#ifdef __vita__
            else
            {
                configSetInt(&resolutionConfig, "MAIN", "SCR_WIDTH", VITA_FULLSCREEN_WIDTH);
                width = VITA_FULLSCREEN_WIDTH;
                configWrite(&resolutionConfig, "f2_res.ini", false);
            }
#endif

            int screenHeight;
            if (configGetInt(&resolutionConfig, "MAIN", "SCR_HEIGHT", &screenHeight)) {
                height = screenHeight;
            }
#ifdef __vita__
            else
            {
                configSetInt(&resolutionConfig, "MAIN", "SCR_HEIGHT", VITA_FULLSCREEN_HEIGHT);
                height = VITA_FULLSCREEN_HEIGHT;
                configWrite(&resolutionConfig, "f2_res.ini", false);
            }
#endif

            bool windowed;
            if (configGetBool(&resolutionConfig, "MAIN", "WINDOWED", &windowed)) {
                fullscreen = !windowed;
            }
#ifdef __vita__
            else
            {
                configSetInt(&resolutionConfig, "MAIN", "WINDOWED", 0);
                fullscreen = 0;
                configWrite(&resolutionConfig, "f2_res.ini", false);
            }
#endif

            configGetBool(&resolutionConfig, "IFACE", "IFACE_BAR_MODE", &gInterfaceBarMode);
            configGetInt(&resolutionConfig, "IFACE", "IFACE_BAR_WIDTH", &gInterfaceBarWidth);
            configGetInt(&resolutionConfig, "IFACE", "IFACE_BAR_SIDE_ART", &gInterfaceSidePanelsImageId);
            configGetBool(&resolutionConfig, "IFACE", "IFACE_BAR_SIDES_ORI", &gInterfaceSidePanelsExtendFromScreenEdge);

#ifdef __vita__
            // load Vita options here
            if (width < DEFAULT_WIDTH) {
                width = DEFAULT_WIDTH;
            }
            if (height < DEFAULT_HEIGHT) {
                height = DEFAULT_HEIGHT;
            }

            int frontTouch;
            if (configGetInt(&resolutionConfig, "VITA", "FRONT_TOUCH_MODE", &frontTouch)) {
                frontTouchpadMode = static_cast<TouchpadMode>(frontTouch);
            }
            else
            {
                configSetInt(&resolutionConfig, "VITA", "FRONT_TOUCH_MODE", 1);
                configWrite(&resolutionConfig, "f2_res.ini", false);
            }

            int rearTouch;
            if (configGetInt(&resolutionConfig, "VITA", "REAR_TOUCH_MODE", &rearTouch)) {
                rearTouchpadMode = static_cast<TouchpadMode>(rearTouch);
            }
            else
            {
                configSetInt(&resolutionConfig, "VITA", "REAR_TOUCH_MODE", 0);
                configWrite(&resolutionConfig, "f2_res.ini", false);
            }

            // Use FACE_BAR_MODE=1 by default on Vita
            if (!configGetBool(&resolutionConfig, "IFACE", "IFACE_BAR_MODE", &gInterfaceBarMode)) {
                configSetInt(&resolutionConfig, "IFACE", "IFACE_BAR_MODE", 1);
                configWrite(&resolutionConfig, "f2_res.ini", false);
            }
#endif
        }
        configFree(&resolutionConfig);
    }

    if (_GNW95_init_window(width, height, fullscreen) == -1) {
        return -1;
    }

    if (directDrawInit(width, height, bpp) == -1) {
        return -1;
    }

    _scr_size.left = 0;
    _scr_size.top = 0;
    _scr_size.right = width - 1;
    _scr_size.bottom = height - 1;

    mmxSetEnabled(true);

    _mouse_blit_trans = NULL;
    _scr_blit = _GNW95_ShowRect;
    _zero_mem = _GNW95_zero_vid_mem;
    _mouse_blit = _GNW95_ShowRect;

    return 0;
}

// 0x4CAECC
int _init_vesa_mode(int width, int height)
{
    return _GNW95_init_mode_ex(width, height, 8);
}

// 0x4CAEDC
int _GNW95_init_window(int width, int height, bool fullscreen)
{
#ifdef __vita__
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return -1;
    }

    vita2d_init();
    vita2d_set_vblank_wait(false);

    gSdlWindow = SDL_CreateWindow(gProgramWindowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    if (gSdlWindow == NULL) {
        return -1;
    }

    vita2d_texture_set_alloc_memblock_type(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW);
    texBuffer = vita2d_create_empty_texture_format(width, height, SCE_GXM_TEXTURE_FORMAT_P8_ABGR);
    palettedTexturePointer = (uint8_t*)(vita2d_texture_get_datap(texBuffer));
    memset(palettedTexturePointer, 0, width * height * sizeof(uint8_t));
    setRenderRect(width, height, fullscreen);

    float resolutionSpeedMod = static_cast<float>(height) / DEFAULT_HEIGHT;

    return 0;
#else
    if (gSdlWindow == NULL) {
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            return -1;
        }

        Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

        if (fullscreen) {
            windowFlags |= SDL_WINDOW_FULLSCREEN;
        }

        gSdlWindow = SDL_CreateWindow(gProgramWindowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
        if (gSdlWindow == NULL) {
            return -1;
        }

        if (!createRenderer(width, height)) {
            destroyRenderer();

            SDL_DestroyWindow(gSdlWindow);
            gSdlWindow = NULL;

            return -1;
        }
    }
#endif

    return 0;
}

// 0x4CAF9C
int directDrawInit(int width, int height, int bpp)
{
    if (gSdlSurface != NULL) {
        unsigned char* palette = directDrawGetPalette();
        directDrawFree();

        if (directDrawInit(width, height, bpp) == -1) {
            return -1;
        }

        directDrawSetPalette(palette);

        return 0;
    }

    gSdlSurface = SDL_CreateRGBSurface(0, width, height, bpp, 0, 0, 0, 0);

    SDL_Color colors[256];
    for (int index = 0; index < 256; index++) {
        colors[index].r = index;
        colors[index].g = index;
        colors[index].b = index;
        colors[index].a = 255;
    }

    SDL_SetPaletteColors(gSdlSurface->format->palette, colors, 0, 256);
#ifdef __vita__
    updateVita2dPalette(colors, 0, 256);
#endif

    return 0;
}

// 0x4CB1B0
void directDrawFree()
{
    if (gSdlSurface != NULL) {
        SDL_FreeSurface(gSdlSurface);
        gSdlSurface = NULL;
    }
}

// 0x4CB310
void directDrawSetPaletteInRange(unsigned char* palette, int start, int count)
{
    if (gSdlSurface != NULL && gSdlSurface->format->palette != NULL) {
        SDL_Color colors[256];

        if (count != 0) {
            for (int index = 0; index < count; index++) {
                colors[index].r = palette[index * 3] << 2;
                colors[index].g = palette[index * 3 + 1] << 2;
                colors[index].b = palette[index * 3 + 2] << 2;
                colors[index].a = 255;
            }
        }

        SDL_SetPaletteColors(gSdlSurface->format->palette, colors, start, count);
#ifdef __vita__
        updateVita2dPalette(colors, start, count);
#else
        SDL_BlitSurface(gSdlSurface, NULL, gSdlTextureSurface, NULL);
#endif
    }
}

// 0x4CB568
void directDrawSetPalette(unsigned char* palette)
{
    if (gSdlSurface != NULL && gSdlSurface->format->palette != NULL) {
        SDL_Color colors[256];

        for (int index = 0; index < 256; index++) {
            colors[index].r = palette[index * 3] << 2;
            colors[index].g = palette[index * 3 + 1] << 2;
            colors[index].b = palette[index * 3 + 2] << 2;
            colors[index].a = 255;
        }

        SDL_SetPaletteColors(gSdlSurface->format->palette, colors, 0, 256);
#ifdef __vita__
        updateVita2dPalette(colors, 0, 256);
#else
        SDL_BlitSurface(gSdlSurface, NULL, gSdlTextureSurface, NULL);
#endif
    }
}

// 0x4CB68C
unsigned char* directDrawGetPalette()
{
    // 0x6ACA24
    static unsigned char palette[768];

    if (gSdlSurface != NULL && gSdlSurface->format->palette != NULL) {
        SDL_Color* colors = gSdlSurface->format->palette->colors;

        for (int index = 0; index < 256; index++) {
            SDL_Color* color = &(colors[index]);
            palette[index * 3] = color->r >> 2;
            palette[index * 3 + 1] = color->g >> 2;
            palette[index * 3 + 2] = color->b >> 2;
        }
    }

    return palette;
}

// 0x4CB850
void _GNW95_ShowRect(unsigned char* src, int srcPitch, int a3, int srcX, int srcY, int srcWidth, int srcHeight, int destX, int destY)
{
    blitBufferToBuffer(src + srcPitch * srcY + srcX, srcWidth, srcHeight, srcPitch, (unsigned char*)gSdlSurface->pixels + gSdlSurface->pitch * destY + destX, gSdlSurface->pitch);

#ifdef __vita__
    renderVita2dFrame(gSdlSurface);
#else
    SDL_Rect srcRect;
    srcRect.x = destX;
    srcRect.y = destY;
    srcRect.w = srcWidth;
    srcRect.h = srcHeight;

    SDL_Rect destRect;
    destRect.x = destX;
    destRect.y = destY;
    SDL_BlitSurface(gSdlSurface, &srcRect, gSdlTextureSurface, &destRect);
#endif
}

// Clears drawing surface.
//
// 0x4CBBC8
void _GNW95_zero_vid_mem()
{
    if (!gProgramIsActive) {
        return;
    }

    unsigned char* surface = (unsigned char*)gSdlSurface->pixels;
    for (int y = 0; y < gSdlSurface->h; y++) {
        memset(surface, 0, gSdlSurface->w);
        surface += gSdlSurface->pitch;
    }

#ifndef __vita__
    SDL_BlitSurface(gSdlSurface, NULL, gSdlTextureSurface, NULL);
#endif
}

int screenGetWidth()
{
    // TODO: Make it on par with _xres;
    return rectGetWidth(&_scr_size);
}

int screenGetHeight()
{
    // TODO: Make it on par with _yres.
    return rectGetHeight(&_scr_size);
}

int screenGetVisibleHeight()
{
    int windowBottomMargin = 0;

    if (!gInterfaceBarMode) {
        windowBottomMargin = INTERFACE_BAR_HEIGHT;
    }
    return screenGetHeight() - windowBottomMargin;
}

static bool createRenderer(int width, int height)
{
    gSdlRenderer = SDL_CreateRenderer(gSdlWindow, -1, 0);
    if (gSdlRenderer == NULL) {
        return false;
    }

    if (SDL_RenderSetLogicalSize(gSdlRenderer, width, height) != 0) {
        return false;
    }

    gSdlTexture = SDL_CreateTexture(gSdlRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (gSdlTexture == NULL) {
        return false;
    }

    Uint32 format;
    if (SDL_QueryTexture(gSdlTexture, &format, NULL, NULL, NULL) != 0) {
        return false;
    }

    gSdlTextureSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, SDL_BITSPERPIXEL(format), format);
    if (gSdlTextureSurface == NULL) {
        return false;
    }

    return true;
}

static void destroyRenderer()
{
    if (gSdlTextureSurface != NULL) {
        SDL_FreeSurface(gSdlTextureSurface);
        gSdlTextureSurface = NULL;
    }

    if (gSdlTexture != NULL) {
        SDL_DestroyTexture(gSdlTexture);
        gSdlTexture = NULL;
    }

    if (gSdlRenderer != NULL) {
        SDL_DestroyRenderer(gSdlRenderer);
        gSdlRenderer = NULL;
    }

#ifdef __vita__
    if ( gSdlWindow != nullptr ) {
        SDL_DestroyWindow( gSdlWindow );
        gSdlWindow = nullptr;
    }

    if ( vitaPaletteSurface != nullptr ) {
        SDL_FreeSurface( vitaPaletteSurface );
        vitaPaletteSurface = nullptr;
    }

    vita2d_fini();

    if ( texBuffer != nullptr ) {
        vita2d_free_texture( texBuffer );
        texBuffer = nullptr;
    }
#endif
}

void handleWindowSizeChanged()
{
    destroyRenderer();
    createRenderer(screenGetWidth(), screenGetHeight());
}

void renderPresent()
{
#ifdef __vita__
    renderVita2dFrame(gSdlSurface);
#else
    SDL_UpdateTexture(gSdlTexture, NULL, gSdlTextureSurface->pixels, gSdlTextureSurface->pitch);
    SDL_RenderClear(gSdlRenderer);
    SDL_RenderCopy(gSdlRenderer, gSdlTexture, NULL, NULL);
    SDL_RenderPresent(gSdlRenderer);
#endif
}

#ifdef __vita__
void renderVita2dFrame(SDL_Surface *surface)
{
    memcpy(palettedTexturePointer, surface->pixels, surface->w * surface->h * sizeof(uint8_t));
    vita2d_start_drawing();
    vita2d_draw_rectangle(0, 0, VITA_FULLSCREEN_WIDTH, VITA_FULLSCREEN_HEIGHT, 0xff000000);
    vita2d_draw_texture_scale(texBuffer, renderRect.x, renderRect.y, (float)(renderRect.w) / surface->w, (float)(renderRect.h) / surface->h);
    vita2d_end_drawing();
    vita2d_swap_buffers();
}

void updateVita2dPalette(SDL_Color *colors, int start, int count)
{
    uint32_t palette32Bit[count];

    if (vitaPaletteSurface == NULL) {
        vitaPaletteSurface = SDL_CreateRGBSurface(0, 1, 1, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    }

    for ( size_t i = 0; i < count; ++i ) {
        palette32Bit[i] = SDL_MapRGBA(vitaPaletteSurface->format, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
    }

    memcpy(vita2d_texture_get_palette(texBuffer) + start * sizeof(uint32_t), palette32Bit, sizeof(uint32_t) * count);
}

void setRenderRect(int width, int height, bool fullscreen)
{
    renderRect.x = 0;
    renderRect.y = 0;
    renderRect.w = width;
    renderRect.h = height;
    vita2d_texture_set_filters(texBuffer, SCE_GXM_TEXTURE_FILTER_POINT, SCE_GXM_TEXTURE_FILTER_POINT);

    if (width != VITA_FULLSCREEN_WIDTH || height != VITA_FULLSCREEN_HEIGHT)	{
        if (fullscreen) {
            //resize to fullscreen
            if ((static_cast<float>(VITA_FULLSCREEN_WIDTH) / VITA_FULLSCREEN_HEIGHT) >= (static_cast<float>(width) / height)) {
                float scale = static_cast<float>(VITA_FULLSCREEN_HEIGHT) / height;
                renderRect.w = width * scale;
                renderRect.h = VITA_FULLSCREEN_HEIGHT;
                renderRect.x = (VITA_FULLSCREEN_WIDTH - renderRect.w) / 2;
            } else {
                float scale = static_cast<float>(VITA_FULLSCREEN_WIDTH) / width;
                renderRect.w = VITA_FULLSCREEN_WIDTH;
                renderRect.h = height * scale;
                renderRect.y = (VITA_FULLSCREEN_HEIGHT - renderRect.h) / 2;
            }

            vita2d_texture_set_filters(texBuffer, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
        } else {
            //center game area
            renderRect.x = (VITA_FULLSCREEN_WIDTH - width) / 2;
            renderRect.y = (VITA_FULLSCREEN_HEIGHT - height) / 2;
        }
    }
}
#endif

} // namespace fallout
