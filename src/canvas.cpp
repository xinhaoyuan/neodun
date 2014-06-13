#include <Exception>
#include <assert.h>

#include "canvas.hpp"

using namespace std;
using namespace ND;

SDL_Window *Canvas::sdl_window = NULL;

Canvas::Canvas(Surface *dst) {
    _dst_surface = dst;
    if (dst != NULL) {
        _dst_renderer = SDL_CreateRenderer(
            sdl_window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        assert(_dst_renderer != NULL);
        SDL_SetRenderTarget(_dst_renderer, _dst_surface->_sdl_texture);
    } else {
        _dst_renderer = SDL_CreateRenderer(
            sdl_window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        assert(_dst_renderer != NULL);
    }
}

Canvas::~Canvas(void) {
    SDL_DestroyRenderer(_dst_renderer);
}

void
Canvas::clear(void) {
    SDL_RenderClear(_dst_renderer);
}

void
Canvas::set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(_dst_renderer, r, g, b, a);
}

void
Canvas::fill_rect(int sx, int sy, int ex, int ey) {
    SDL_Rect r;
    r.x = sx; r.y = sy; r.w = ex - sx; r.h = ey - sy;
    SDL_RenderFillRect(_dst_renderer, &r);
}

void
Canvas::flush(void) {
    SDL_RenderPresent(_dst_renderer);
}
