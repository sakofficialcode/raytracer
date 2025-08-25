#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const pointV& center, double radius) : center(center), radius (std::fmax(0,radius)) {}

        bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override {
            vec3 r2c = center - r.origin();
            double a = dot(r.direction(), r.direction());
            double w = dot(r.direction(), r2c);
            double c = dot(r2c, r2c) - radius * radius;
            double discriminant = w * w - a * c;
            if (discriminant < 0) return false;

            double root = (w - std::sqrt(discriminant))/ a; 
            
            if (tmin >= root || root >= tmax) {
                root = (w + std::sqrt(discriminant)) / a;    
                if (tmin >= root || root >= tmax) return false;
            }

            rec.t = root;
            rec.p = r.at(root);

            
            vec3 out_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, out_normal);

            return true;
        }

    private:
        pointV center;
        double radius;

};

#endif