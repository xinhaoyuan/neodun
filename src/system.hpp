#ifndef __ND_INPUT_HPP__
#define __ND_INPUT_HPP__

#include <vector>
#include <map>

#include <SDL.h>

namespace ND {
    
    class System {
    private:
        Uint32 _ticks;

    public:
        System(void);
        ~System(void);

        void next_frame(int fps);
        void process_events(void);

    private:

        int   _n_game_key;
        bool *_key_present;
        bool *_key_pressed;
        
        int *_os_key_hook_present;
        int *_os_key_hook_pressed;
        
    public:

        enum KeyRegisterType {
            KEY_REGISTER_PRESENT,
            KEY_REGISTER_PRESSED,
        };

        void init_game_key(int n_game_key);
        void register_key(int os_key, int game_key, KeyRegisterType type);
        bool key_present(int game_keycode);
        bool key_pressed(int game_keycode);
    };
}

#endif
