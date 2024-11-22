#ifndef FALLOUT_TOUCH_H_
#define FALLOUT_TOUCH_H_

#include <SDL.h>

namespace fallout {

enum GestureType {
    kUnrecognized,
    kTap,
    kLongPress,
    kPan,
};

enum GestureState {
    kPossible,
    kBegan,
    kChanged,
    kEnded,
};

#ifdef __vita__
enum TouchpadMode {
    kTouchDisabled = 0,
    kTouchDirect = 1,
    kTouchTrackpad = 2
};
#endif

struct Gesture {
    GestureType type;
    GestureState state;
    int numberOfTouches;
    int x;
    int y;
};

void touch_handle_start(SDL_TouchFingerEvent* event);
void touch_handle_move(SDL_TouchFingerEvent* event);
void touch_handle_end(SDL_TouchFingerEvent* event);
void touch_process_gesture();
bool touch_get_gesture(Gesture* gesture);

} // namespace fallout

#endif /* FALLOUT_TOUCH_H_ */
