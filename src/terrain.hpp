#ifndef __ND_TERRAIN_HPP__
#define __ND_TERRAIN_HPP__

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "sprite.hpp"

namespace ND {

    class Game;
    class TerrainBlock;
    
    class Terrain {
        int _width;
        int _height;

        TerrainBlock **_map;
        Sprite       **_sprite_map;
        Game          *_game;
        
    public:
        static const int BLOCK_WIDTH;
        static const int BLOCK_YHEIGHT;
        static const int BLOCK_ZHEIGHT;
        
        Terrain(void);
        ~Terrain(void);
        
        void setup(Game *game);
        void clear(void);
        void tick(void);

    private:
        static std::map<std::string, Terrain *> _cache;

    public:
        static Terrain *get_from_map_file(const char *filename);
    };

    class TerrainBlock {
    private:

        Surface *_src;
        std::vector< std::pair< int, int > > _frames_src;
        int _frame_counter;
        int _tick_counter;

        static const int TICKS_PER_FRAME;
        
        TerrainBlock(void);
        ~TerrainBlock(void);

    public:

        void init(Sprite *spirte);
        void tick(Sprite *sprite);

    private:
        static std::map<std::string, TerrainBlock *> *_dir;
        
    public:
        static TerrainBlock *get_from_name(const char *name);
    };
    
}

#endif
