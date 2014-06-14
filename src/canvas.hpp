#ifndef __ND_CANVAS_HPP__
#define __ND_CANVAS_HPP__

#include <map>
#include <string>
#include <SDL.h>

namespace ND {

    class Surface;
    
    class Canvas {
    private:

        static SDL_Renderer *_renderer;

        Surface *_dst_surface;
        int _height, _width;

        friend class Surface;
        
    public:
        static SDL_Window *sdl_window;

        static void global_init(void);

        Canvas(Surface *dst);
        ~Canvas(void);

        int height(void);
        int width(void);

        void begin(void);
        void end(void);
        
        void clear(void);
        void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void fill_rect(int sx, int sy, int ex, int ey);
        void copy_surface(Surface *src, int sx, int sy, int width, int height, int ox, int oy);
    };

    class Surface {
    private:
        friend class Canvas;
        SDL_Texture *_sdl_texture;
        
    public:

        Surface(void);
        ~Surface(void);

    private:
        static std::map<std::string, Surface *> _cache;
        
    public:
        static Surface *get_from_image_file(const char *filename);
    };

    class Viewport {
    public:

        int sx, sy, ex, ey;
        
    };
    
}

#endif
