#ifndef __ND_TERRAIN_HPP__
#define __ND_TERRAIN_HPP__

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "sprite.hpp"
#include "collision.hpp"
#include "controller.hpp"

namespace ND {

    class Game;
    class TerrainBlock;
    
    class Terrain {
        int _width;
        int _height;

        TerrainBlock    **_map;
        Sprite          **_sprite_map;
        Game             *_game;

        class Controller : public ND::Controller {
        private:
            friend class Terrain;
            Terrain *_;
            
        public:
            virtual int  priority(void);
            virtual void tick(Game *game);
        };

        Controller _controller;
        
    public:
        
        Terrain(void);
        ~Terrain(void);
        
        void setup(Game *game);
        void clear(void);
        bool collision_test(CollisionObject *object);

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
        bool _can_pass;

        static const int TICKS_PER_FRAME;
        
        TerrainBlock(void);
        ~TerrainBlock(void);

    public:

        void init(Sprite *spirte);
        void tick(Sprite *sprite);

        bool can_pass(void);

    private:
        static std::map<std::string, TerrainBlock *> *_dir;
        
    public:
        static TerrainBlock *get_from_name(const char *name);
    };
    
}

#endif
