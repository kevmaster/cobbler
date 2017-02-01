#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include "sprites.h"
#include "renderFunctions.h"
#include "text.h"

#define MS_PER_UPDATE 5

int main(int argc, char* argv[]) {


    SDL_Window *window;                    // Declare a pointer
    SDL_Renderer *renderer;

    setMode(spriteMode);               //
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    int err = SDL_CreateWindowAndRenderer(
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL,         // flags - see below
        &window,
        &renderer
    );
    SDL_SetRenderDrawColor(renderer,0,255,0,255); 
    // Check that the window was successfully created
    if (err != 0) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    int txterror = initText();
    if(txterror)
        printf("text couldn't be initialized with error: %i",txterror);
    //printf("rendering text\n");
    SDL_Surface* stuff = renderText("HELLO",0,0,256,256,256);
    sprite*  textsprite = createSprite(stuff, renderer, 0, 0);
    addSprite(&friends,textsprite);
    // printf("done rendering text\n");
    // The window is open: could enter program loop here (see SDL_PollEvent())
    double previous = time(NULL);
    double lag = 0.0;
    short quit = 0;
    while(!quit){
        //do some calculations to limit fps
        double current = time(NULL);
        double elapsed = current - previous;
        previous = current;
        lag+=elapsed;
        //check the events for user input
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = 1;
                    continue;
                    break;
                case SDL_MOUSEMOTION:
                    //textsprite->rendRect->x=event.motion.x;
                    //textsprite->rendRect->y=event.motion.y;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.repeat == 1)
                        break;
                    switch( event.key.keysym.sym ){
                        case SDLK_UP:
                            textsprite->vy--;
                            break;
                        case SDLK_DOWN:
                            textsprite->vy++;
                            break;
                        case SDLK_LEFT:
                            textsprite->vx--;
                            break;
                        case SDLK_RIGHT:
                            textsprite->vx++;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    if(event.key.repeat == 1)
                        break;
                    switch( event.key.keysym.sym ){
                        case SDLK_UP:
                            textsprite->vy++;
                            break;
                        case SDLK_DOWN:
                            textsprite->vy--;
                            break;
                        case SDLK_LEFT:
                            textsprite->vx++;
                            break;
                        case SDLK_RIGHT:
                            textsprite->vx--;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                   // printf("unhandled event");
                    break;
            }
        }
        //update the game state a bunch
        while (lag >= MS_PER_UPDATE)
        {
            //update game stuff
            //updateSprites(friends);
            lag -= MS_PER_UPDATE;
        }
        updateSprites(friends);
        //render the game
        SDL_RenderClear(renderer);
        render(renderer);
        SDL_RenderPresent(renderer);
    }


    //SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
