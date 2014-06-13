#include "system.hpp"

using namespace ND;

InputEvent::InputEvent(SDL_Event *sdl_event) {
    if (sdl_event->type == SDL_KEYDOWN)
        _type = KEY_DOWN;
}

InputEvent::~InputEvent(void) {
}

InputEvent::Type InputEvent::get_type(void) {
    return _type;
}

System::System(void) {
    _ticks = SDL_GetTicks();
}

System::~System(void) {
}

bool
System::has_event(void) {
    return SDL_PollEvent(NULL);
}

InputEvent *
System::acquire_event(void) {
    SDL_Event event;
    InputEvent *result = NULL;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN)
            result = new InputEvent(&event);
    }
    return result;
}

void
System::release_event(InputEvent *event) {
    delete event;
}

void
System::delay_for_fps(int fps) {
    // delay for FPS
    int nticks = SDL_GetTicks();
    nticks = 1000 / fps - SDL_TICKS_PASSED(nticks, _ticks);
    if (nticks > 0)
        SDL_Delay(nticks);
    _ticks = nticks;
}
