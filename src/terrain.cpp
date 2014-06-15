#include <iostream>
#include <fstream>
#include <string>

#include "terrain.hpp"
#include "game.hpp"

using namespace std;
using namespace ND;

const int Terrain::BLOCK_WIDTH   = 32;
const int Terrain::BLOCK_YHEIGHT = 20;
const int Terrain::BLOCK_ZHEIGHT = 12;

#define TERRAIN_BLOCK_DATA_FILE "../data/tb.txt"

Terrain::Terrain(void) {
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
                s->width    = BLOCK_WIDTH;
                s->height   = BLOCK_YHEIGHT + BLOCK_ZHEIGHT;
            } else smap[index] = NULL;
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
                _game->sprite_insert(_sprite_map[index]);
            _map[index]->init(_sprite_map[index]);
        }
    }
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
}

void
Terrain::tick(void) {
    for (int y = 0; y < _height; ++ y) {
        for (int x = 0; x < _width; ++ x) {
            int index = y * _width + x;
            if (_map[index] != NULL)
                _map[index]->tick(_sprite_map[index]);
        }
    }
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

TerrainBlock::TerrainBlock() {
    _frame_counter = 0;
    _tick_counter = 0;
}

TerrainBlock::~TerrainBlock() {
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
