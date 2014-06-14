#ifndef __ND_GAME_HPP__
#define __ND_GAME_HPP__

#include <set>
#include <map>

#include "canvas.hpp"
#include "system.hpp"
#include "sprite.hpp"

namespace ND {
    
    class Game {
    private:        
        bool              _running;
        Canvas           *_canvas;
        System           *_system;
        
        void _init(void);
        
    public:
        Game(Canvas *canvas, System *system);
        ~Game(void);

        void main(void);

        // sence management
    private:
        class SpriteDrawOrder {
        public:
            bool operator()(Sprite *a, Sprite *b);
        };
        typedef std::set<Sprite *, SpriteDrawOrder> OrderedSpriteSet;
        
        OrderedSpriteSet  _sprites;
        Viewport          _viewport;

    public:
        void sprite_insert(Sprite *sprite);
        void sprite_remove(Sprite *sprite);        

        // resource management
    private:
        std::map<std::string, Surface *> _image_surfaces;

    public:
        Surface *load_surface_from_image(const char *filename);
    };
}

#endif
