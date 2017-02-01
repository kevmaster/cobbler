#ifndef RENDERFUNCTIONS_H
#define RENDERFUNCTIONS_H

#include "sprites.h"

typedef int (*renderFunc) (SDL_Renderer* renderer);

extern sprite* enemies;
extern sprite* friends;
extern sprite* neutrals;

int spriteMode(SDL_Renderer* renderer);
//could use an ENUM here instead of keeping the render function public
int setMode(renderFunc newMode);


//the actual render function
int render(SDL_Renderer* renderer);


#endif
