#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class colorV;

class material {
    public:
        virtual bool scatter (const ray& r_in, const hit_record& rec, colorV& attenuation, ray& scattered) const {
            return false;
        }
};

class diffuse : public material {
    public:
        diffuse(const colorV& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, colorV& attenuation, ray& scattered) const override {
            vec3 scatter_direction = rec.norm + rand_unit_vec();

            if (scatter_direction.near_zero()) scatter_direction = rec.norm;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        colorV albedo;
};

class metal : public material {
    public:
        metal(const colorV& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, colorV& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.norm);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return true;

        }
    private:
        colorV albedo;
};

#endif