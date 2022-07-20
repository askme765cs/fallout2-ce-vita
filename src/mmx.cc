#include "mmx.h"

#include "core.h"

#include <string.h>

#ifdef OPENMP_BLIT
#include <omp.h>
#endif

// Return `true` if CPU supports MMX.
//
// 0x4E08A0
bool mmxIsSupported()
{
    return SDL_HasMMX() == SDL_TRUE;
}

// 0x4E0DB0
void mmxBlit(unsigned char* dest, int destPitch, unsigned char* src, int srcPitch, int width, int height)
{
    if (gMmxEnabled) {
        // TODO: Blit with MMX.
        gMmxEnabled = false;
        mmxBlit(dest, destPitch, src, srcPitch, width, height);
        gMmxEnabled = true;
    } else {
#ifdef OPENMP_BLIT
        #pragma omp parallel for
        for (int y = 0; y < height; y++) {
            unsigned char* thread_dest = dest + y * destPitch;
            unsigned char* thread_src = src + y * srcPitch;
            memcpy(thread_dest, thread_src, width);
        }
#else
        for (int y = 0; y < height; y++) {
            memcpy(dest, src, width);
            dest += destPitch;
            src += srcPitch;
        }
#endif
    }
}

// 0x4E0ED5
void mmxBlitTrans(unsigned char* dest, int destPitch, unsigned char* src, int srcPitch, int width, int height)
{
    if (gMmxEnabled) {
        // TODO: Blit with MMX.
        gMmxEnabled = false;
        mmxBlitTrans(dest, destPitch, src, srcPitch, width, height);
        gMmxEnabled = true;
    } else {
#ifdef OPENMP_BLIT
        #pragma omp parallel for
        for (int y = 0; y < height; y++) {
            unsigned char* thread_src = src + y * srcPitch;
            unsigned char* thread_dest = dest + y * destPitch;
            for (int x = 0; x < width; x++) {
                unsigned char c = *thread_src++;
                if (c != 0) {
                    *thread_dest = c;
                }
                thread_dest++;
            }
        }
#else
        int destSkip = destPitch - width;
        int srcSkip = srcPitch - width;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                unsigned char c = *src++;
                if (c != 0) {
                    *dest = c;
                }
                dest++;
            }
            src += srcSkip;
            dest += destSkip;
        }
#endif
    }
}
