#include <iostream>
#include <stdint.h>
#include <assert.h>

#include "game.hpp"
#include "demo.hpp"

using namespace std;
using namespace ND;

bool
Game::SpriteDrawOrder::operator()(Sprite *a, Sprite *b) {
    if (a->layer != b->layer)
        return a->layer < b->layer;

    if (a->center_y != b->center_y)
        return a->center_y < b->center_y;
    
    return ((uintptr_t)a < (uintptr_t)b);
}

bool
Game::ControllerOrder::operator()(Controller *a, Controller *b) {
    if (a->priority() != b->priority())
        return a->priority() < b->priority();

    return ((uintptr_t)a < (uintptr_t)b);
}

Game::Game(Canvas *canvas, System *system) {
    _canvas = canvas;
    _system = system;
    _running = true;
    _terrain = NULL;
}

Game::~Game(void) {
}

System *
Game::system(void) {
    return _system;
}

void
Game::sprite_add(Sprite *sprite) {
    _sprites.insert(sprite);
}

void
Game::sprite_remove(Sprite *sprite) {
    _sprites.erase(sprite);
}

void
Game::controller_add(Controller *controller) {
    _controllers.insert(controller);
}

void
Game::controller_remove(Controller *controller) {
    _controllers.erase(controller);
}

void
Game::_init(void) {
    init_demo(this);
}

Terrain *
Game::terrain(void) {
    return _terrain;
}

void
Game::terrain_set(Terrain *terrain) {
    if (_terrain != NULL) {
        _terrain->clear();
    }

    _terrain = terrain;
    _terrain->setup(this);
}

void
Game::viewport_move_center_to(int x, int y) {
    int width = _canvas->width();
    int height = _canvas->height();

    _viewport.sx = x - width / 2;
    _viewport.sy = y - height / 2;
    _viewport.ex = _viewport.sx + width;
    _viewport.ey = _viewport.sy + width;
}

void
Game::main(void) {
    _viewport.sx = 0;
    _viewport.sy = 0;
    _viewport.ex = _canvas->width();
    _viewport.ey = _canvas->height();

    _init();
    
    while (_running) {
        _system->next_frame(GAME_FPS);

        // external event
        _system->process_events();

        if (_system->quit_requested() || _system->key_pressed(GAME_KEY_ESC))
            _running = false;
        
        // internal event
        for (OrderedControllerSet::iterator it = _controllers.begin();
             it != _controllers.end(); ++ it) {
            (*it)->tick(this);
        }

        _canvas->begin();
        _canvas->set_color(0, 0, 0, 255);
        _canvas->clear();

        // draw all sprites from far to near
        for (OrderedSpriteSet::iterator it = _sprites.begin();
             it != _sprites.end(); ++ it) {
            (*it)->draw(_canvas, &_viewport);
        }

        _canvas->end();
    }
}
