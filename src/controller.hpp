#ifndef __ND_CONTROLLER_HPP__
#define __ND_CONTROLLER_HPP__

namespace ND {

    class Game;

    class Controller {
    public:
        virtual int  priority() = 0;
        virtual void tick(Game *game) = 0;
    };

}

#endif
