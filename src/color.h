#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

class colorV : public vec3 {
    public:
        colorV() : vec3() {}
        colorV(const vec3& v) : vec3(v.x(), v.y(), v.z()) {}
        colorV(double e0, double e1, double e2) : vec3(e0, e1, e2) {}

        // Convert to RGB values
        int r() const { return int(255.999 * x()); }
        int g() const { return int(255.999 * y()); }
        int b() const { return int(255.999 * z()); }
};

#endif