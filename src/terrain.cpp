#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "terrain.hpp"
#include "game.hpp"

using namespace std;
using namespace ND;

Terrain::Terrain(void) {
    _controller._ = this;
}

Terrain::~Terrain(void) {
}

map<string, Terrain *> Terrain::_cache;

Terrain *
Terrain::get_from_map_file(const char *filename) {
    if (_cache.find(filename) != _cache.end()) {
        return _cache[filename];
    }
    
    ifstream in(filename);
    int width, height;
    string block_name;
    
    in >> width >> height;
    TerrainBlock **map = new TerrainBlock*[width * height];
    Sprite **smap = new Sprite*[width * height];
    
    for (int y = 0; y < height; ++ y) {
        for (int x = 0; x < width; ++ x) {
            int index = y * width + x;
            in >> block_name;

            smap[index] = NULL;
            
            if (block_name == "_")
                map[index] = NULL;
            else map[index] = TerrainBlock::get_from_name(block_name.c_str());
            
            if (map[index] != NULL) {
                Sprite *s = new Sprite();
                smap[index] = s;

                s->offset_x = x * BLOCK_WIDTH;
                s->offset_y = y * BLOCK_YHEIGHT;
                s->center_x = x * BLOCK_WIDTH + BLOCK_WIDTH / 2;
                s->center_y = y * BLOCK_YHEIGHT + BLOCK_YHEIGHT / 2 + BLOCK_ZHEIGHT;
                if (map[index]->can_pass())
                    s->layer = LAYER_CEIL;
                else s->layer = LAYER_CHARACTER;
                s->width    = BLOCK_WIDTH;
                s->height   = BLOCK_YHEIGHT + BLOCK_ZHEIGHT;
            }
        }
    }

    Terrain *result = new Terrain();
    result->_width = width;
    result->_height = height;
    result->_map = map;
    result->_sprite_map = smap;
    result->_game = NULL;

    _cache[filename] = result;

    return result;
}

void
Terrain::setup(Game *game) {
    _game = game;
    for (int y = 0; y < _height; ++ y) {
        for (int x = 0; x < _width; ++ x) {
            int index = y * _width + x;
            if (_map[index] != NULL)
                _game->sprite_add(_sprite_map[index]);
            _map[index]->init(_sprite_map[index]);
        }
    }

    _game->controller_add(&_controller);
}

void
Terrain::clear(void) {
    for (int y = 0; y < _height; ++ y) {
        for (int x = 0; x < _width; ++ x) {
            int index = y * _width + x;
            if (_map[index] != NULL)
                _game->sprite_remove(_sprite_map[index]);
        }
    }

    _game->controller_remove(&_controller);
}

int
Terrain::Controller::priority(void) {
    return 0;
}

void
Terrain::Controller::tick(Game *game) {
    for (int y = 0; y < _->_height; ++ y) {
        for (int x = 0; x < _->_width; ++ x) {
            int index = y * _->_width + x;
            if (_->_map[index] != NULL)
                _->_map[index]->tick(_->_sprite_map[index]);
        }
    }
}

bool
Terrain::collision_test(CollisionObject *object) {
    int bb_sx = (object->type == CollisionObject::COLLISION_BOX) ?
        object->sx : floor(object->center_x - object->radius);
    int bb_sy = (object->type == CollisionObject::COLLISION_BOX) ?
        object->sy : floor(object->center_y - object->radius);
    int bb_ex = (object->type == CollisionObject::COLLISION_BOX) ?
        object->ex : ceil(object->center_x + object->radius);
    int bb_ey = (object->type == CollisionObject::COLLISION_BOX) ?
        object->ey : ceil(object->center_y + object->radius);
    
    bb_sx = bb_sx / BLOCK_WIDTH;
    bb_sy = bb_sy / BLOCK_YHEIGHT;
    bb_ex = (bb_ex + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
    bb_ey = (bb_ey + BLOCK_YHEIGHT - 1) / BLOCK_YHEIGHT;

    if (bb_sx < 0) bb_sx = 0;
    if (bb_ex > _width) bb_ex = _width;
    if (bb_sy < 0) bb_sy = 0;
    if (bb_ey > _height) bb_ey = _height;

    CollisionObject cur_block;
    cur_block.type = CollisionObject::COLLISION_BOX;
    for (int y = bb_sy; y < bb_ey; ++ y) {
        for (int x = bb_sx; x < bb_ex; ++ x) {
            int index = y * _width + x;
            if (_map[index] != NULL && !_map[index]->can_pass()) {
                cur_block.sx = x * BLOCK_WIDTH;
                cur_block.sy = y * BLOCK_YHEIGHT;
                cur_block.ex = x * BLOCK_WIDTH   + BLOCK_WIDTH;
                cur_block.ey = y * BLOCK_YHEIGHT + BLOCK_YHEIGHT;
                
                if (cur_block.test(object)) return true;
            }
        }
    }

    return false;
}

map<string, TerrainBlock *> *TerrainBlock::_dir = NULL;

TerrainBlock *
TerrainBlock::get_from_name(const char *name) {
    if (_dir == NULL) {
        _dir = new map<string, TerrainBlock *>();
        
        ifstream in(TERRAIN_BLOCK_DATA_FILE);
        int n_blocks;
        
        in >> n_blocks;

        for (int i = 0; i < n_blocks; ++ i) {
            string name;
            string surface_filename;
            in >> name;
            TerrainBlock *block = new TerrainBlock();
            in >> block->_can_pass;
            in >> surface_filename;
            block->_src = Surface::get_from_image_file(surface_filename.c_str());
            int n_frames;
            in >> n_frames;
            for (int f = 0; f < n_frames; ++ f) {
                int src_x, src_y;
                in >> src_x >> src_y;
                block->_frames_src.push_back(pair<int, int>(src_x, src_y));
            }

            (*_dir)[name] = block;
        }
    }

    if (_dir->find(name) == _dir->end()) return NULL;
    else return (*_dir)[name];
}

TerrainBlock::TerrainBlock(void) {
    _frame_counter = 0;
    _tick_counter = 0;
}

TerrainBlock::~TerrainBlock(void) {
}

const int TerrainBlock::TICKS_PER_FRAME = 15;

void
TerrainBlock::init(Sprite *sprite) {
    sprite->source = _src;
    sprite->src_x  = _frames_src[_frame_counter].first;
    sprite->src_y  = _frames_src[_frame_counter].second;
}

void
TerrainBlock::tick(Sprite *sprite) {
    if (++ _tick_counter >= TICKS_PER_FRAME) {
        _frame_counter = (_frame_counter + 1) % _frames_src.size();
        _tick_counter = 0;
    }
    
    sprite->src_x  = _frames_src[_frame_counter].first;
    sprite->src_y  = _frames_src[_frame_counter].second;
}

bool
TerrainBlock::can_pass(void) {
    return _can_pass;
}
