#include <iostream>
#include <SDL.h>

#include "canvas.hpp"
#include "game.hpp"

using namespace std;
using namespace ND;

int
main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        cerr << "Cannot init SDL: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_Window *win = SDL_CreateWindow("Main", 100, 100, 640, 480,
                                       SDL_WINDOW_SHOWN);
    
    if (win == NULL) {
        cerr << "Cannot create SDL window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    Canvas::sdl_window = win;
    Canvas::global_init();

    Canvas *canvas = new Canvas(NULL);
    System *system = new System();

    system->init_game_key(N_GAME_KEY);
    system->register_key(SDL_SCANCODE_ESCAPE,
                         GAME_KEY_ESC,
                         System::KEY_REGISTER_PRESSED);
    system->register_key(SDL_SCANCODE_UP,
                         GAME_KEY_UP,
                         System::KEY_REGISTER_PRESENT);
    system->register_key(SDL_SCANCODE_DOWN,
                         GAME_KEY_DOWN,
                         System::KEY_REGISTER_PRESENT);
    system->register_key(SDL_SCANCODE_LEFT,
                         GAME_KEY_LEFT,
                         System::KEY_REGISTER_PRESENT);
    system->register_key(SDL_SCANCODE_RIGHT,
                         GAME_KEY_RIGHT,
                         System::KEY_REGISTER_PRESENT);

    
    Game *game = new Game(canvas, system);

    game->main();

    delete game;
    delete system;
    delete canvas;
    
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
