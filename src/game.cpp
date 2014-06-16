#include <iostream>
#include <stdint.h>
#include <assert.h>

#include "game.hpp"

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

class TestController : public Controller {
    Sprite          *_sprite;
    CollisionObject *_collision;
    
public:
    TestController(Game *game) {
        // create sprite manually
        Surface *surface = Surface::get_from_image_file("../gfx/sample_1.png");
        Sprite *sprite = new Sprite();
        sprite->source = surface;
        sprite->src_x = sprite->src_y = 0;
        sprite->width = sprite->height = 32;
        sprite->offset_x = sprite->offset_y = 100;
        sprite->center_x = sprite->center_y = 120;
        sprite->layer    = LAYER_CHARACTER;
        _sprite = sprite;
        _collision = CollisionObject::create_box(100, 100, 132, 120);

        game->sprite_add(_sprite);
    }

    ~TestController(void) {
        delete _sprite;
        delete _collision;
    }
    
    virtual int priority(void) {
        return 0;
    }

    virtual void tick(Game *game) {
        int dx = 0, dy = 0;
        
        if (game->system()->key_present(GAME_KEY_UP)) {
            dy = -1;
        }

        if (game->system()->key_present(GAME_KEY_DOWN)) {
            dy = 1;
        }

        if (game->system()->key_present(GAME_KEY_LEFT)) {
            dx = -1;
        }

        if (game->system()->key_present(GAME_KEY_RIGHT)) {
            dx = 1;
        }

        _collision->sx += dx;
        _collision->sy += dy;
        _collision->ex += dx;
        _collision->ey += dy;

        if (game->terrain()->collision_test(_collision)) {
            _collision->sx -= dx;
            _collision->sy -= dy;
            _collision->ex -= dx;
            _collision->ey -= dy;
        } else {
            game->sprite_remove(_sprite);
            
            _sprite->center_x += dx;
            _sprite->center_y += dy;
            _sprite->offset_x += dx;
            _sprite->offset_y += dy;
            
            game->sprite_add(_sprite);
        }
    }
};

void
Game::_init(void) {
    Terrain *terrain = Terrain::get_from_map_file("../data/map_1.txt");
    terrain_set(terrain);

    controller_add(new TestController(this));
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

        if (_system->key_pressed(GAME_KEY_ESC))
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
