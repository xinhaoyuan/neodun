#ifndef __ND_CANVAS_HPP__
#define __ND_CANVAS_HPP__

#include <SDL.h>

namespace ND {

    class Surface;
    
    class Canvas {
    private:
        
        Surface *_dst_surface;
        SDL_Renderer *_dst_renderer;
        
    public:
        static SDL_Window *sdl_window;

        Canvas(Surface *dst);
        ~Canvas(void);

        void flush(void);
        void clear(void);
        void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void fill_rect(int sx, int sy, int ex, int ey);
    };

    class Surface {
    private:
        friend class Canvas;
        SDL_Texture *_sdl_texture;

    public:

        Surface *create(int width, int height);
    };
    
}

#endif
