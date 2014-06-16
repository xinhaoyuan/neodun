#ifndef __ND_SPRITE_HPP__
#define __ND_SPRITE_HPP__

#include "canvas.hpp"

namespace ND {

    class Sprite {
    public:
        
        Surface *source;
        int src_x, src_y, width, height;
        int offset_x, offset_y;
        int center_x, center_y;

        bool in_viewport(Viewport *viewport);
        void draw(Canvas *canvas, Viewport *viewport);
    };

    class Game;

    class SpriteController {
    public:
        virtual int  priority() = 0;
        virtual void tick(Game *game) = 0;
    };
    
}

#endif
