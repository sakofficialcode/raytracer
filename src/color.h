#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

class colorV : public vec3 {
    public:
        colorV() : vec3() {}
        colorV(const vec3& v) : vec3(v.x(), v.y(), v.z()) { }
        colorV(double e0, double e1, double e2) : vec3(e0, e1, e2) {}

        inline double linear_to_gamma(double linear_component)
        {
            if (linear_component > 0)
                return std::sqrt(linear_component);

            return 0;
        }
        double getR() {
            double tempR = linear_to_gamma(x());
            if (tempR > 1) return 1;
            if (tempR < 0) return 0;
            return tempR;
        }
        double getG() {
            double tempG = linear_to_gamma(y());
            if (tempG > 1) return 1;
            if (tempG < 0) return 0;
            return tempG;
        }
        double getB() {
            double tempB = linear_to_gamma(z());
            if (tempB > 1) return 1;
            if (tempB < 0) return 0;
            return tempB;
        }

        
        
        
        // Convert to RGB values
        int r() { return int(255.999 * getR()); }
        int g() { return int(255.999 * getG()); }
        int b() { return int(255.999 * getB()); }
};


#endif