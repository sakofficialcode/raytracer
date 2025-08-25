#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

#include "utils.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

constexpr double aspect_ratio = 16.0/9.0;
constexpr int image_width = 1280;
constexpr int image_height = (int)(image_width/aspect_ratio);
constexpr int samples_per_pixel = 1;
constexpr int max_depth = 50;

constexpr double view_height = 2.0;
constexpr double view_width = view_height * (double) (image_width) / image_height;
constexpr double focal_length = 1.0;
vec3 cam_loc = pointV(0,0,0);


vec3 viewport_u = vec3(view_width, 0, 0);
vec3 viewport_v = vec3(0, -view_height, 0);

vec3 pixel_delta_u = viewport_u / image_width;
vec3 pixel_delta_v = viewport_v / image_height;

pointV viewport_upper_left = cam_loc - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
pointV pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

static SDL_FPoint points[image_width * image_height];

hittable_list world;

colorV ray_color(const ray& r, int depth, const hittable& world) {

    if (depth <= 0) return colorV(0,0,0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        vec3 direction = vec_on_surface(rec.norm) + rec.norm;
        return 0.5 * (ray_color(ray(rec.p, direction), depth-1, world));
    }
    

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (colorV)((1.0-a)*colorV(1.0, 1.0, 1.0) + a*colorV(0.5, 0.7, 1.0));
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

    

    world.add(std::make_shared<sphere>(pointV(0,0,-1), 0.5));
    world.add(std::make_shared<sphere>(pointV(0,-100.5,-1), 100));

    SDL_RenderClear(renderer);


    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            colorV pC = colorV(0,0,0);
            //auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            //auto ray_direction = pixel_center - cam_loc;
            for (int s = 0; s < samples_per_pixel; s++) {
                auto offset = vec3(Random::getRand(0.0,1.0) - 0.5, Random::getRand(0.0,1.0) - 0.5, 0);
                auto pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

                auto ray_origin = pointV(0,0,0);
                auto ray_direction = pixel_sample - ray_origin;

                ray r(ray_origin, ray_direction);
                pC += ray_color(r, max_depth, world);
            }
            
            //ray r(cam_loc, ray_direction);

            
            pC /= samples_per_pixel;
            SDL_SetRenderDrawColor(renderer, pC.r(), pC.g(), pC.b(), SDL_ALPHA_OPAQUE); 
            SDL_RenderPoint(renderer, points[i + j * image_width].x, points[i + j * image_width].y);
        }
    }
    


    SDL_RenderPresent(renderer); 


    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{

    // SDL_RenderClear(renderer);


    // for (int j = 0; j < image_height; j++) {
    //     for (int i = 0; i < image_width; i++) {
    //         colorV pC = colorV(0,0,0);
    //         //auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    //         //auto ray_direction = pixel_center - cam_loc;
    //         for (int s = 0; s < samples_per_pixel; s++) {
    //             auto offset = vec3(Random::getRand(0,1) - 0.5, Random::getRand(0,1) - 0.5, 0);
    //             auto pixel_sample = pixel00_loc
    //                             + ((i + offset.x()) * pixel_delta_u)
    //                             + ((j + offset.y()) * pixel_delta_v);

    //             auto ray_origin = pointV(0,0,0);
    //             auto ray_direction = pixel_sample - ray_origin;

    //             ray r(ray_origin, ray_direction);
    //             pC += ray_color(r, world);
    //         }
            
    //         //ray r(cam_loc, ray_direction);

            
    //         pC /= samples_per_pixel;
    //         SDL_SetRenderDrawColor(renderer, pC.r(), pC.g(), pC.b(), SDL_ALPHA_OPAQUE); 
    //         SDL_RenderPoint(renderer, points[i + j * image_width].x, points[i + j * image_width].y);
    //     }
    // }
    


    // SDL_RenderPresent(renderer);  

    return SDL_APP_CONTINUE; 
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) {}