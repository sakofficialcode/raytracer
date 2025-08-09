#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const pointV& origin, const vec3& direction) : orig(origin), dir(direction) {}

        const pointV& origin() const { return orig; }
        const vec3& direction() const { return dir; }

        pointV at (double t) const { return orig + t * dir; }

    private:
        pointV orig;
        vec3 dir;
};

#endif