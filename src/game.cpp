#include <iostream>
#include <stdint.h>

#include <SDL_image.h>

#include "game.hpp"

using namespace std;
using namespace ND;

#define GAME_FPS 30

bool
SpriteDrawOrder::operator()(Sprite *a, Sprite *b) {
    if (a->center_y < b->center_y) return true;
    else return ((uintptr_t)a < (uintptr_t)b);
}

Game::Game(Canvas *canvas, System *system) {
    _canvas = canvas;
    _system = system;
    _running = true;
    _sprites = new OrderedSpriteSet();
}

Game::~Game(void) {
    delete _sprites;
}

void
Game::sprite_insert(Sprite *sprite) {
    _sprites->insert(sprite);
}

void
Game::sprite_remove(Sprite *sprite) {
    _sprites->erase(sprite);
}

void
Game::_init(void) {
    // create sprite manually
    Surface *surface = Surface::create_from_image("../gfx/sample_1.png");
    Sprite *sprite = new Sprite();
    sprite->source = surface;
    sprite->src_x = sprite->src_y = 0;
    sprite->width = sprite->height = 32;
    sprite->offset_x = sprite->offset_y = 10;
    sprite->center_x = sprite->center_y = 0;

    sprite_insert(sprite);
}

void
Game::main(void) {
    _viewport.sx = 0;
    _viewport.sy = 0;
    _viewport.ex = _canvas->width();
    _viewport.ey = _canvas->height();

    _init();
    
    while (_running) {
        _system->delay_for_fps(GAME_FPS);

        // external event
        while (_system->has_event()) {
            InputEvent *e = _system->acquire_event();
            if (e == NULL) continue;
            
            switch (e->get_type()) {
            case InputEvent::KEY_DOWN:
                _running = false;
                break;
            }
            _system->release_event(e);
        }

        _canvas->begin();
        _canvas->set_color(0, 0, 0, 255);
        _canvas->clear();

        // draw all sprites from far to near
        for (OrderedSpriteSet::iterator it = _sprites->begin();
             it != _sprites->end(); ++ it) {
            (*it)->draw(_canvas, &_viewport);
        }

        _canvas->end();
    }
}
