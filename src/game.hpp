#ifndef __ND_GAME_HPP__
#define __ND_GAME_HPP__

#include <set>

#include "canvas.hpp"
#include "system.hpp"
#include "sprite.hpp"

namespace ND {

    class SpriteDrawOrder {
    public:
        bool operator()(Sprite *a, Sprite *b);
    };
    
    class Game {
    private:

        typedef std::set<Sprite *, SpriteDrawOrder> OrderedSpriteSet;
        
        bool              _running;
        Canvas           *_canvas;
        System           *_system;
        OrderedSpriteSet *_sprites;
        Viewport          _viewport;

        void _init(void);
        
    public:
        Game(Canvas *canvas, System *system);
        ~Game(void);

        void sprite_insert(Sprite *sprite);
        void sprite_remove(Sprite *sprite);

        void main(void);
    };
}

#endif
