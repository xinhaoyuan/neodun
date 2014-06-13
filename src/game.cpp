#include <iostream>

#include "game.hpp"

using namespace std;
using namespace ND;

#define GAME_FPS 30

Game::Game(Canvas *canvas, System *system) {
    _canvas = canvas;
    _system = system;
    _running = true;
}

Game::~Game(void) {
}

void
Game::main(void) {
    // clear the window with black
    _canvas->set_color(0, 0, 0, 255);
    _canvas->clear();
    // draw a white rectangle
    _canvas->set_color(255, 255, 255, 255);
    _canvas->fill_rect(10, 10, 100, 100);
    _canvas->flush();    

    while (_running) {
        _system->delay_for_fps(GAME_FPS);
        cerr << "Heartbeat!\n";
        
        // external event
        while (_system->has_event()) {
            InputEvent *e = _system->acquire_event();
            if (e == NULL) continue;
            
            switch (e->get_type()) {
            case InputEvent::KEY_DOWN:
                _running = false;
                break;
            }
            _system->release_event(e);
        }
    }
}
