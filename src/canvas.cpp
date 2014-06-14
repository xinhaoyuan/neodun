#include <assert.h>
#include <SDL_image.h>
#include <iostream>

#include "canvas.hpp"

using namespace std;
using namespace ND;

SDL_Window *Canvas::sdl_window = NULL;
SDL_Renderer *Canvas::_renderer = NULL;

void
Canvas::global_init(void) {
    assert(_renderer == NULL);
    _renderer = SDL_CreateRenderer(
        sdl_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    assert(_renderer != NULL);
}

Canvas::Canvas(Surface *dst) {
    _dst_surface = dst;
    if (dst == NULL)
        SDL_GetWindowSize(sdl_window, &_width, &_height);
    else SDL_QueryTexture(dst->_sdl_texture, NULL, NULL,
                          &_width, &_height);
}

Canvas::~Canvas(void) {
}

int
Canvas::width(void) {
    return _width;
}

int
Canvas::height(void) {
    return _height;
}

void
Canvas::clear(void) {
    SDL_RenderClear(_renderer);
}

void
Canvas::set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
}

void
Canvas::fill_rect(int sx, int sy, int ex, int ey) {
    SDL_Rect r;
    r.x = sx; r.y = sy; r.w = ex - sx; r.h = ey - sy;
    SDL_RenderFillRect(_renderer, &r);
}

void
Canvas::begin(void) {
    SDL_SetRenderTarget(_renderer, _dst_surface ? _dst_surface->_sdl_texture : NULL);
}

void
Canvas::end(void) {
    SDL_RenderPresent(_renderer);
}

void
Canvas::copy_surface(Surface *src, int sx, int sy, int width, int height, int ox, int oy) {
    SDL_Rect src_rect, dst_rect;
    src_rect.x = sx; src_rect.y = sy; src_rect.w = width; src_rect.h = height;
    dst_rect.x = ox; dst_rect.y = oy; dst_rect.w = width; dst_rect.h = height;

    SDL_RenderCopy(_renderer, src->_sdl_texture, &src_rect, &dst_rect);
}

Surface *
Surface::create_from_image(const char *filename) {
    Surface *result = new Surface();
    SDL_Surface *surface = IMG_Load(filename);
    assert(surface != NULL);
    result->_sdl_texture = SDL_CreateTextureFromSurface(Canvas::_renderer, surface);
    assert(result->_sdl_texture != NULL);
    SDL_FreeSurface(surface);

    return result;
}

Surface::Surface() {
}

Surface::~Surface() {
    SDL_DestroyTexture(_sdl_texture);
}
