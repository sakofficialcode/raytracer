#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        pointV p;
        vec3 norm;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& out_normal) {
            front_face = dot(r.direction(), out_normal) < 0;
            norm = front_face ? out_normal : -out_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const = 0;

};

#endif