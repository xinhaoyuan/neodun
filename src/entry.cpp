#include <iostream>
#include <SDL.h>

using namespace std;

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

    SDL_Delay(2000);
    
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
