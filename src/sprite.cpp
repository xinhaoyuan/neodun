#include "sprite.hpp"
#include <iostream>

using namespace std;
using namespace ND;

bool
Sprite::in_viewport(Viewport *viewport) {
    if (offset_x >= viewport->ex) return false;
    if (offset_y >= viewport->ey) return false;
    if (offset_x + width < viewport->sx) return false;
    if (offset_y + height < viewport->sy) return false;
    return true;
}

void
Sprite::draw(Canvas *canvas, Viewport *viewport) {
    if (!in_viewport(viewport)) return;
    canvas->copy_surface(source, src_x, src_y, width, height, offset_x - viewport->sx, offset_y - viewport->sy);
}
