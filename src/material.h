#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "random.h"

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

class dielectric : public material {
  public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, colorV& attenuation, ray& scattered)
    const override {
        attenuation = colorV(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.norm), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > Random::getRand(0.0,0.999) )
            direction = reflect(unit_direction, rec.norm);
        else
            direction = refract(unit_direction, rec.norm, ri);

        scattered = ray(rec.p, direction);
        return true;
    }

  private:
    double refraction_index;

    static double reflectance(double cos, double ri) {
        auto F0 = (ri - 1) / (ri + 1 );
        F0 = F0*F0;
        return F0 + (1-F0)*std::pow((1 - cos),5);
    }
};

#endif