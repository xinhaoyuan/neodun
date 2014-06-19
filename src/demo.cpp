#include "controller.hpp"
#include "game.hpp"
#include "sprite.hpp"
#include "collision.hpp"
#include "demo.hpp"

using namespace ND;

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

    bool test_move(Game *game, int dx, int dy) {
        _collision->sx += dx;
        _collision->sy += dy;
        _collision->ex += dx;
        _collision->ey += dy;

        bool result = game->terrain()->collision_test(_collision);
        
        _collision->sx -= dx;
        _collision->sy -= dy;
        _collision->ex -= dx;
        _collision->ey -= dy;

        return !result;
    }

    int try_move(Game *game, int dx, int dy, int step) {
        int mx = dx * step;
        int my = dy * step;
        if (test_move(game, mx, my)) {

            _collision->sx += mx;
            _collision->sy += my;
            _collision->ex += mx;
            _collision->ey += my;

            game->sprite_remove(_sprite);
            
            _sprite->center_x += mx;
            _sprite->center_y += my;
            _sprite->offset_x += mx;
            _sprite->offset_y += my;
            
            game->sprite_add(_sprite);

            return step;
        } else {
            int t;
            for (t = 1; t < step; ++ t) {
                if (test_move(game, dx * t, dy * t) == false) break;
            }

            -- t;

            if (t) {
                mx = dx * t;
                my = dy * t;

                _collision->sx += mx;
                _collision->sy += my;
                _collision->ex += mx;
                _collision->ey += my;

                game->sprite_remove(_sprite);
            
                _sprite->center_x += mx;
                _sprite->center_y += my;
                _sprite->offset_x += mx;
                _sprite->offset_y += my;
            
                game->sprite_add(_sprite);
            }
            
            return t;
        }
    }

    virtual void tick(Game *game) {
        int dx = 0, dy = 0;
        int step = 3;
        
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

        int f = try_move(game, dx, dy, step);
        if (f < step) {
            if (dx) try_move(game, dx, 0, step - f);
            if (dy) try_move(game, 0, dy, step - f);
        }
        
        game->viewport_move_center_to(_sprite->center_x, _sprite->center_y);
    }
};

void
ND::init_demo(Game *game) {
    Terrain *terrain = Terrain::get_from_map_file("../data/map_1.txt");
    game->terrain_set(terrain);

    game->controller_add(new TestController(game));
}
