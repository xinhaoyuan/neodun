#ifndef __ND_COLLISION_HPP__
#define __ND_COLLISION_HPP__

namespace ND {

    class CollisionObject {
    public:
        enum Type {
            COLLISION_BOX,
            COLLISION_CIRCLE
        };

        Type type;
        union {
            struct {
                double radius;
                double center_x, center_y;
            };

            struct {
                int sx, sy;
                int ex, ey;
            };
        };
        
        static CollisionObject *create_circle(double x, double y, double radius);
        static CollisionObject *create_box(int sx, int sy, int ex, int ey);
        
        bool test(CollisionObject *object);
    };
    
}

#endif
