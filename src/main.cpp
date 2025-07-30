#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

constexpr int image_width = 256;  
constexpr int image_height = 256;

static SDL_FPoint points[image_width * image_height];


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Primitives", "1.0", "com.example.renderer-primitives");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/primitives", image_width, image_height, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* set up some random points */
    for (int i = 0; i < SDL_arraysize(points); i++) {
        points[i].x = i % image_width;
        points[i].y = i / image_width;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* dark gray, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */


    /* draw some points across the canvas. */
    for (int i = 0; i < SDL_arraysize(points); i++) {

        auto r = double(points[i].x) / (image_width-1);
        auto g = double(points[i].y) / (image_height-1);
        auto b = 0.0;

        int ir = int(255.999 * r);
        int ig = int(255.999 * g);
        int ib = int(255.999 * b);

        SDL_SetRenderDrawColor(renderer, ir, ig, ib, SDL_ALPHA_OPAQUE);  /* red, full alpha */
        SDL_RenderPoint(renderer, points[i].x, points[i].y);  /* draw a point at the given coordinates */
    }
    


    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}