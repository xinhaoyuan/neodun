#include <iostream>
#include <fstream>
#include <string>

#include "terrain.hpp"

using namespace std;
using namespace ND;

const int Terrain::BLOCK_WIDTH   = 32;
const int Terrain::BLOCK_YHEIGHT = 20;
const int Terrain::BLOCK_ZHEIGHT = 12;

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
            
            if (map[index] != NULL)
                smap[index] = new Sprite();
            else smap[index] = NULL;
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
        ifstream in(name);
        // XXX
    }

    if (_dir->find(name) == _dir->end()) return NULL;
    else return (*_dir)[name];
}

void
TerrainBlock::init(Sprite *sprite) {
}

void
TerrainBlock::tick(Sprite *sprite) {
}
