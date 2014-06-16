#include <math.h>
#include <assert.h>

#include "collision.hpp"

using namespace ND;

static inline double
_distance_sqr(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

CollisionObject *
CollisionObject::create_box(int sx, int sy, int ex, int ey) {
    CollisionObject *ret = new CollisionObject();
    ret->type = COLLISION_BOX;
    ret->sx = sx;
    ret->sy = sy;
    ret->ex = ex;
    ret->ey = ey;
    return ret;
}

CollisionObject *
CollisionObject::create_circle(double x, double y, double radius) {
    CollisionObject *ret = new CollisionObject();
    ret->type = COLLISION_CIRCLE;
    ret->center_x = x;
    ret->center_y = y;
    ret->radius = radius;
    return ret;
}

bool
CollisionObject::test(CollisionObject *object) {
    if (type == COLLISION_BOX && object->type == COLLISION_BOX) {
        int min_x = object->sx; if (sx > min_x) min_x = sx;
        int max_x = object->ex; if (ex < max_x) max_x = ex;
        if (min_x >= max_x) return false;

        int min_y = object->sx; if (sx > min_y) min_y = sx;
        int max_y = object->ex; if (ex < max_y) max_y = ex;
        if (min_y >= max_y) return false;

        return true;
    } else if (type == COLLISION_CIRCLE && object->type == COLLISION_CIRCLE) {
        double distance_sqr = _distance_sqr(center_x, center_y, object->center_x, object->center_y);
        return distance_sqr < (radius + object->radius) * (radius + object->radius);
    } else if (type == COLLISION_CIRCLE) return object->test(this);
    
    assert(type == COLLISION_BOX && object->type == COLLISION_CIRCLE);
    // self is box and object is circle
    // check bounding box first
    double min_x = object->center_x - object->radius;
    if (sx > min_x) min_x = sx;
    double max_x = object->center_x + object->radius;
    if (ex < max_x) max_x = ex;
    if (min_x >= max_x) return false;
    // case by case
    double tx, ty;
    if (object->center_x < sx)
        tx = sx;
    else if (object->center_y > ex)
        tx = ex;
    else return true;

    if (object->center_y < sy)
        ty = sy;
    else if (object->center_y > ey)
        ty = ey;
    else return true;
    
    return (_distance_sqr(tx, tx, object->center_x, object->center_y) <
            object->radius * object->radius);    
}
