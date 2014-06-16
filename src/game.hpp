#ifndef __ND_GAME_HPP__
#define __ND_GAME_HPP__

#include <set>
#include <map>

#include "canvas.hpp"
#include "system.hpp"
#include "sprite.hpp"
#include "terrain.hpp"

namespace ND {

    const int GAME_FPS = 30;

    enum {
        GAME_KEY_ESC = 0,
        N_GAME_KEY
    };
    
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
        System *system(void);

        // sence management
    private:
        class SpriteDrawOrder {
        public:
            bool operator()(Sprite *a, Sprite *b);
        };
        typedef std::set<Sprite *, SpriteDrawOrder> OrderedSpriteSet;

        class ControllerOrder {
        public:
            bool operator()(Controller *a, Controller *b);
        };
        typedef std::set<Controller *, ControllerOrder> OrderedControllerSet;

        OrderedControllerSet  _controllers;
        OrderedSpriteSet      _sprites;
        Viewport              _viewport;
        Terrain              *_terrain;

    public:
        void sprite_add(Sprite *sprite);
        void sprite_remove(Sprite *sprite);
        void controller_add(Controller *controller);
        void controller_remove(Controller *controller);
        void terrain_set(Terrain *terrain);
    };
}

#endif
