#include "sprites.h"

//renders the list of sprites
int renderSprites(sprite* sprites, SDL_Renderer* renderer){
	if(sprites == NULL){
		return 0;
	}
	sprite* current = sprites;
	while(current){
		SDL_RenderCopy(renderer,current->texture, NULL, current->rendRect);
		current = current->next;
	}
	return 0;
}
//adds a sprite to the beginning of the sprite list
int addSprite(sprite** sprites, sprite* newSprite){
	if(*sprites == NULL){
		*sprites = newSprite;
		return 0;
	}
	newSprite->next = *sprites;
	*sprites = newSprite;
	return 0;
}
//remove a sprite from the list
int deleteSprite(sprite** sprites, sprite* delSprite){
	sprite* current = *sprites;
	if(current == delSprite){
		*sprites = (*sprites)->next;
		return 0;
	}
	while(current){
		if(current->next == delSprite){
			current->next = delSprite->next;
			return 0;
		}
	}
	return -1;
}
//update sprites based on speed and direction
int updateSprites(sprite* sprites){
	if(sprites == NULL){
		return 0;
	}
	sprite* current = sprites;
	while(current){
		if(current->vx+current->vy != 0){ //why did i do this? this causes an issue where i cant move diagonally sometimes
			current->rendRect->x += current->vx;
			current->rendRect->y += current->vy;
		}
		current = current->next;
	}
	return 0;
}
sprite* createSprite(SDL_Surface* surface, SDL_Renderer* renderer, int x, int y){
	sprite* newSprite = malloc(sizeof(sprite));
	newSprite->texture = SDL_CreateTextureFromSurface(renderer,surface);
	if(newSprite->texture == NULL){
		printf("failed to create texture from surface\n");
		printf("%s",SDL_GetError());
		free(newSprite);
		return NULL;
	}
	newSprite->vx = 0;
	newSprite->vy = 0;
	newSprite->next = NULL;
	newSprite->rendRect = malloc(sizeof(SDL_Rect));
	newSprite->rendRect->x=x;
	newSprite->rendRect->y=y;
	newSprite->rendRect->w=surface->w;
	newSprite->rendRect->h=surface->h;
	return newSprite;

}
int destroySprite(sprite* delSprite){
	if(delSprite==NULL)
		return 0;
	free(delSprite->rendRect);
	free(delSprite);
	return 0;
}