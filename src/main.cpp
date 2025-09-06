#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <memory>
#include <cmath> // For sin/cos


#include "utils.h"

#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "camera.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

constexpr double aspect_ratio = 16.0/9.0;
constexpr int image_width = 630;
constexpr int image_height = (int)(image_width/aspect_ratio);
constexpr int samples_per_pixel = 10;
constexpr int max_depth = 8;

Camera camera(
    pointV(13,2,3),
    pointV(0,0,0),       // lookat
    vec3(0,1,0),           // vup
    20,                    // vfov
    aspect_ratio,
    image_width,
    samples_per_pixel,
    max_depth
);

pointV cam_position = camera.lookfrom;
pointV cam_lookat = camera.lookat;
bool camera_dirty = true; 

static SDL_FPoint points[image_width * image_height];

hittable_list world;

colorV ray_color(const ray& r, int depth, const hittable& world) {

    if (depth <= 0) return colorV(0,0,0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        /*
        vec3 direction = vec_on_surface(rec.norm) + rec.norm;
        return 0.5 * (ray_color(ray(rec.p, direction), depth-1, world));
        */

        ray scattered;
        colorV attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth -1, world);
        }
        return colorV(0,0,0);
    }
    

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (colorV)((1.0-a)*colorV(1.0, 1.0, 1.0) + a*colorV(0.5, 0.7, 1.0));
}

void render_scene() {
    camera.lookfrom = cam_position;
    camera.lookat = cam_lookat;
    camera.initialize();

    SDL_RenderClear(renderer);

    std::vector<colorV> framebuffer(image_width * camera.get_image_height());

    for (int j = 0; j < camera.get_image_height(); ++j) {
        for (int i = 0; i < camera.image_width; ++i) {
            colorV pixel_color(0,0,0);
            
            for (int s = 0; s < camera.samples_per_pixel; ++s) {
                ray r = camera.get_ray(i, j);
                pixel_color += ray_color(r, camera.max_depth, world);
                
            }
            
            pixel_color *= camera.get_pixel_samples_scale();
            framebuffer[i + j * image_width] = pixel_color;
        }

            
        
    }


    for (int i = 0; i < SDL_arraysize(points); i++) {
        colorV pixel_color = framebuffer[i];
        int j = i / image_width;
        int w = i % image_width;

                    SDL_SetRenderDrawColor(renderer, pixel_color.r(), pixel_color.g(), pixel_color.b(), SDL_ALPHA_OPAQUE); 
            SDL_RenderPoint(renderer, points[w + j * image_width].x, points[w + j * image_width].y);
    }

    SDL_RenderPresent(renderer);
    
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Ray Tracer", "1.0", "com.example.ray-tracer");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Ray Tracer", image_width, image_height, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    
    for (int i = 0; i < SDL_arraysize(points); i++) {
        points[i].x = i % image_width;
        points[i].y = i / image_width;
    }

    // auto material_ground = std::make_shared<diffuse>(colorV(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<diffuse>(colorV(0.1, 0.2, 0.5));
    // auto material_left   = std::make_shared<dielectric>(1.50);
    // auto material_bubble = std::make_shared<dielectric>(1.00 / 1.50);
    // auto material_right  = std::make_shared<metal>(colorV(0.8, 0.6, 0.2), 1.0);

    // world.add(std::make_shared<sphere>(pointV( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(std::make_shared<sphere>(pointV( 0.0,    0.0, -1.2),   0.5, material_center));
    // world.add(std::make_shared<sphere>(pointV(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(std::make_shared<sphere>(pointV(-1.0,    0.0, -1.0),   0.4, material_bubble));
    // world.add(std::make_shared<sphere>(pointV( 1.0,    0.0, -1.0),   0.5, material_right));

    

    auto ground_material = std::make_shared<diffuse>(colorV(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(pointV(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = Random::getRand(0.0, 1.0);
            pointV center(a + 0.9*Random::getRand(0.0,1.0), 0.2, b + 0.9*Random::getRand(0.0,1.0));

            if ((center - pointV(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = colorV(rand_vec(0.0,1.0) * rand_vec(0.0,1.0));
                    sphere_material = std::make_shared<diffuse>(albedo);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = colorV(rand_vec(0.5, 1));
                    auto fuzz = Random::getRand(0.0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(pointV(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<diffuse>(colorV(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(pointV(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(colorV(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(pointV(4, 1, 0), 1.0, material3));



    SDL_RenderClear(renderer);


    render_scene();
    


    //SDL_RenderPresent(renderer); 


    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    static const double move_step = 0.2;
    static const double angle_step = 0.04; // radians, ~2.3 degrees

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        vec3 move_vec(0, 0, 0);
        bool rotated = false;

        switch (event->key.scancode) {
            // Movement
            case SDL_SCANCODE_W: move_vec = vec3(0, 0, -move_step); break;
            case SDL_SCANCODE_S: move_vec = vec3(0, 0, move_step);  break;
            case SDL_SCANCODE_A: move_vec = vec3(-move_step, 0, 0); break;
            case SDL_SCANCODE_D: move_vec = vec3(move_step, 0, 0);  break;
            case SDL_SCANCODE_Q: move_vec = vec3(0, move_step, 0);  break;
            case SDL_SCANCODE_E: move_vec = vec3(0, -move_step, 0); break;

            // Look direction: horizontal (yaw)
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_RIGHT: {
                // Vector from lookfrom to lookat
                vec3 look_dir = cam_lookat - cam_position;
                // Up vector (usually y axis)
                vec3 up = vec3(0, 1, 0);
                // Yaw: rotate around up axis
                double theta = (event->key.scancode == SDL_SCANCODE_LEFT) ? angle_step : -angle_step;
                // Rodrigues' rotation formula
                vec3 look_dir_rot =
                    look_dir * std::cos(theta) +
                    cross(up, look_dir) * std::sin(theta) +
                    up * dot(up, look_dir) * (1 - std::cos(theta));
                cam_lookat = cam_position + look_dir_rot;
                rotated = true;
                break;
            }
            // Look direction: vertical (pitch)
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_DOWN: {
                // Vector from lookfrom to lookat
                vec3 look_dir = cam_lookat - cam_position;
                // Right vector (perpendicular to up and look_dir)
                vec3 up = vec3(0, 1, 0);
                vec3 right = unit_vector(cross(look_dir, up));
                double theta = (event->key.scancode == SDL_SCANCODE_UP) ? angle_step : -angle_step;
                // Rodrigues' rotation formula
                vec3 look_dir_rot =
                    look_dir * std::cos(theta) +
                    cross(right, look_dir) * std::sin(theta) +
                    right * dot(right, look_dir) * (1 - std::cos(theta));
                cam_lookat = cam_position + look_dir_rot;
                rotated = true;
                break;
            }
        }
        if (move_vec.length_squared() > 0) {
            cam_position = cam_position + move_vec;
            cam_lookat = cam_lookat + move_vec; // Move lookat with camera for FPS-style movement
            camera_dirty = true;
        }
        if (rotated) {
            camera_dirty = true;
        }
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    if (camera_dirty) {
        render_scene();
        camera_dirty = false;
    }
    return SDL_APP_CONTINUE; 
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) {}

// Set true to render on first launch

