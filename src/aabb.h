#ifndef AABB_H
#define AABB_H


#include <algorithm>
#include <limits>
#include "ray.h"

class aabb {
public:
    pointV minimum;
    pointV maximum;
    aabb() : minimum(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()), maximum(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()) {}
    aabb(const pointV& a, const pointV& b) : minimum(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z)), maximum(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z)) {}

    bool hit(const ray& r, double t_min, double t_max) const {
        for (int d = 0; d < 3; d++) {
            double invD = 1.0f / r.direction()[d];
            double t0 = (minimum[d] - r.origin()[d]) * invD;
            double t1 = (maximum[d] - r.origin()[d]) * invD;
            if (invD < 0.0f) std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    int longest_axis() const {
        bool diff[3] = {maximum[0] - minimum[0], maximum[1] - minimum[1], maximum[2] - minimum[2]};
        int longest = 0;
        if (diff[1] > diff[longest]) longest = 1;
        if (diff[2] > diff[longest]) longest = 2;
        return longest;
    }





}

inline aabb surrounding_box(aabb box0, aabb box1) {
    pointV small(fmin(box0.minimum.x, box1.minimum.x),
                 fmin(box0.minimum.y, box1.minimum.y),
                 fmin(box0.minimum.z, box1.minimum.z));
    pointV big(fmax(box0.maximum.x, box1.maximum.x),
               fmax(box0.maximum.y, box1.maximum.y),
               fmax(box0.maximum.z, box1.maximum.z));
    return aabb(small, big);
}
#endif