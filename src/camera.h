#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
#include "random.h"
#include "utils.h"

class Camera {
public:
    // Public camera settings
    double aspect_ratio      = 1.0;
    int    image_width       = 100;
    int    samples_per_pixel = 10;
    int    max_depth         = 10;

    double vfov     = 90;              // Vertical field of view in degrees
    pointV lookfrom = pointV(0,0,0);
    pointV lookat   = pointV(0,0,-1);
    vec3   vup      = vec3(0,1,0);

    Camera() { initialize(); }

    Camera(
        pointV lookfrom,
        pointV lookat,
        vec3 vup,
        double vfov,
        double aspect_ratio,
        int image_width,
        int samples_per_pixel = 10,
        int max_depth = 10
    ) : lookfrom(lookfrom), lookat(lookat), vup(vup), vfov(vfov),
        aspect_ratio(aspect_ratio), image_width(image_width),
        samples_per_pixel(samples_per_pixel), max_depth(max_depth)
    {
        initialize();
    }

    // Call this if you change any public settings after construction
    void initialize() {
        image_height = int(image_width / aspect_ratio);
        if (image_height < 1) image_height = 1;

        pixel_samples_scale = 1.0 / samples_per_pixel;
        center = lookfrom;

        // Viewport dimensions
        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2.0 * h * focal_length;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Camera frame basis
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Viewport edges
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Pixel deltas
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Upper left pixel location
        auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    // Get a ray through pixel (i, j), with random subpixel sampling
    ray get_ray(int i, int j) const {
        auto offset = vec3(Random::getRand(0.0,1.0) - 0.5, Random::getRand(0.0,1.0) - 0.5, 0);
        auto pixel_sample = pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);

        auto ray_direction = pixel_sample - center;
        return ray(center, ray_direction);
    }

    int get_image_height() const { return image_height; }
    double get_pixel_samples_scale() const { return pixel_samples_scale; }

private:
    int    image_height = 0;
    double pixel_samples_scale = 1.0;
    pointV center;
    pointV pixel00_loc;
    vec3   pixel_delta_u, pixel_delta_v;
    vec3   u, v, w;
};

#endif