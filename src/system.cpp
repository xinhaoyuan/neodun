#include "system.hpp"

using namespace ND;

System::System(void) {
    _ticks = SDL_GetTicks();
    _n_game_key = -1;
}

System::~System(void) {
}

void
System::process_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            int present = _os_key_hook_present[event.key.keysym.scancode];
            int pressed = _os_key_hook_pressed[event.key.keysym.scancode];
            if (present >= 0)
                _key_present[present] = true;
            if (pressed >= 0)
                _key_pressed[pressed] = true;
        } else if (event.type == SDL_KEYUP) {
            int present = _os_key_hook_present[event.key.keysym.scancode];
            if (present >= 0)
                _key_present[present] = false;
        }
    }
}

void
System::next_frame(int fps) {
    for (int i = 0; i < _n_game_key; ++ i)
        _key_pressed[i] = false;
    
    // delay for FPS
    int nticks = SDL_GetTicks();
    nticks = 1000 / fps - SDL_TICKS_PASSED(nticks, _ticks);
    if (nticks > 0)
        SDL_Delay(nticks);
    _ticks = nticks;
}

void
System::init_game_key(int n_game_key) {
    if (_n_game_key >= 0) return;
    _n_game_key = n_game_key;
    
    _key_present = new bool[_n_game_key];
    _key_pressed = new bool[_n_game_key];

    _os_key_hook_present = new int[SDL_NUM_SCANCODES];
    _os_key_hook_pressed = new int[SDL_NUM_SCANCODES];

    memset(_os_key_hook_present, -1, sizeof(int) * SDL_NUM_SCANCODES);
    memset(_os_key_hook_pressed, -1, sizeof(int) * SDL_NUM_SCANCODES);

    memset(_key_present, 0, sizeof(bool) * _n_game_key);
    memset(_key_pressed, 0, sizeof(bool) * _n_game_key);
}

void
System::register_key(int os_key, int game_key, KeyRegisterType type) {
    if (type == KEY_REGISTER_PRESENT) {
        _os_key_hook_present[os_key] = game_key;
    } else if (type == KEY_REGISTER_PRESSED) {
        _os_key_hook_pressed[os_key] = game_key;
    }
}

bool
System::key_present(int game_key) {
    return _key_present[game_key];
}

bool
System::key_pressed(int game_key) {
    return _key_pressed[game_key];
}
