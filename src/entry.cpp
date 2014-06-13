#include <iostream>
#include <SDL.h>

#include "canvas.hpp"

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

    Canvas *canvas = new Canvas(NULL);
    // clear the window with black
    canvas->set_color(0, 0, 0, 255);
    canvas->clear();
    // draw a white rectangle
    canvas->set_color(255, 255, 255, 255);
    canvas->fill_rect(10, 10, 100, 100);
    canvas->flush();
    
    SDL_Delay(2000);

    delete canvas;
    
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
