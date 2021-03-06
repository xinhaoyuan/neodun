#ifndef __ND_SPRITE_HPP__
#define __ND_SPRITE_HPP__

#include "canvas.hpp"

namespace ND {
    
    class Sprite {
    public:
        
        Surface *source;
        int src_x, src_y, width, height;
        int offset_x, offset_y;
        int center_x, center_y, layer;

        bool in_viewport(Viewport *viewport);
        void draw(Canvas *canvas, Viewport *viewport);
    };
    
}

#endif
