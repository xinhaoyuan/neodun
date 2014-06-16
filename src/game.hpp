#ifndef __ND_GAME_HPP__
#define __ND_GAME_HPP__

#include <set>
#include <map>

#include "canvas.hpp"
#include "system.hpp"
#include "sprite.hpp"
#include "terrain.hpp"

namespace ND {
    
    class Game {
    private:        
        bool    _running;
        Canvas *_canvas;
        System *_system;

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

        class SpriteControllerOrder {
        public:
            bool operator()(SpriteController *a, SpriteController *b);
        };
        typedef std::set<SpriteController *, SpriteControllerOrder> OrderedSpriteControllerSet;

        OrderedSpriteControllerSet
                          _sprite_controllers;
        OrderedSpriteSet  _sprites;
        Viewport          _viewport;
        Terrain          *_terrain;

    public:
        void sprite_add(Sprite *sprite);
        void sprite_remove(Sprite *sprite);
        void sprite_controller_add(SpriteController *controller);
        void sprite_controller_remove(SpriteController *controller);
        void terrain_set(Terrain *terrain);
    };
}

#endif
