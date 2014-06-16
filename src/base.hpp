#ifndef __ND_BASE_HPP__
#define __ND_BASE_HPP__

namespace ND {
    
    enum SpriteLayer {
        LAYER_CEIL      = 0,
        LAYER_CHARACTER = 1,
        LAYER_EFFECT    = 2
    };

    static const int GAME_FPS = 30;
    
    enum GameKey {
        GAME_KEY_ESC = 0,
        GAME_KEY_UP,
        GAME_KEY_DOWN,
        GAME_KEY_LEFT,
        GAME_KEY_RIGHT,
        N_GAME_KEY
    };

    static const int BLOCK_WIDTH   = 32;
    static const int BLOCK_YHEIGHT = 20;
    static const int BLOCK_ZHEIGHT = 12;

    static const char *TERRAIN_BLOCK_DATA_FILE = "../data/tb.txt";

}

#endif
