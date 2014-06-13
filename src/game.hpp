#ifndef __ND_GAME_HPP__
#define __ND_GAME_HPP__

#include "canvas.hpp"
#include "system.hpp"

namespace ND {

    class Game {
    private:
        
        bool _running;
        Canvas *_canvas;
        System *_system;
        
    public:
        Game(Canvas *canvas, System *system);
        ~Game(void);
        
        void main(void);
    };
}

#endif
