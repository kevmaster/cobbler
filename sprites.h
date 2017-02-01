#ifndef SPRITES_H
#define SPRITES_H
#include <SDL2/SDL.h>

//sprites are just for rendering purposes and don't include other important character/enemy information

typedef struct sprite {
	SDL_Texture *texture;
	SDL_Rect *rendRect;
	int vx;
	int vy;
	struct sprite* next;
} sprite;

//renders the list of sprites
int renderSprites(sprite* sprites, SDL_Renderer* renderer);
//adds a sprite to the sprite list
int addSprite(sprite **sprites, sprite* newSprite);
//remove a sprite from the list
int deleteSprite(sprite **sprites, sprite* delSprite);
//update sprites based on speed and direction
int updateSprites(sprite* sprites);
//create new sprite (pointer should be empty)
sprite* createSprite(SDL_Surface* surface, SDL_Renderer* renderer, int x, int y);
//delete sprite
int destroySprite(sprite* delSprite);

#endif
