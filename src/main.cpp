#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "color.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

constexpr double aspect_ratio = 16.0/9.0;
constexpr int image_width = 256;
constexpr int image_height = (int)(image_width/aspect_ratio);

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

double hit_sphere(const ray& r, const pointV& center, double radius)
{
    vec3 r2c = center - r.origin();
    double a = dot(r.direction(), r.direction());
    double w = dot(r.direction(), r2c);
    double c = dot(r2c, r2c) - radius * radius;
    double discriminant = w * w - a * c;
    if (discriminant < 0) return -1.0;
    else {
        return (w - std::sqrt(discriminant))/a;
    }
}

colorV ray_color(const ray& r) {

    double t = hit_sphere(r, pointV(0,0,-1), 0.5);
    if (t > 0){
        vec3 N = unit_vector(r.at(t) - pointV(0,0,-1));      
        
        return 0.5*colorV(N.x()+1, N.y()+1, N.z()+1);
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

    SDL_RenderClear(renderer);


    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - cam_loc;
            ray r(cam_loc, ray_direction);

            colorV pC = ray_color(r);

            SDL_SetRenderDrawColor(renderer, pC.r(), pC.g(), pC.b(), SDL_ALPHA_OPAQUE); 
            SDL_RenderPoint(renderer, points[i + j * image_width].x, points[i + j * image_width].y);
        }
    }
    


    SDL_RenderPresent(renderer);  

    return SDL_APP_CONTINUE; 
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) {}