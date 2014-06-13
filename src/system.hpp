#ifndef __ND_INPUT_HPP__
#define __ND_INPUT_HPP__

#include <SDL.h>

namespace ND {

    class InputEvent {
    public:
        enum Type {
            KEY_DOWN
        };

    private:
        Type _type;
        friend class System;

    public:
        InputEvent(SDL_Event *sdl_event);
        ~InputEvent();
        
        Type get_type();
    };
    
    class System {
    private:
        Uint32 _ticks;
        
    public:
        System();
        ~System();

        void delay_for_fps(int ms);
        bool has_event(void);
        InputEvent *acquire_event(void);
        void release_event(InputEvent *e);
    };
}

#endif
