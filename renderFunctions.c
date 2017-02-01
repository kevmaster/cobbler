#include "renderFunctions.h"
#include "text.h"

renderFunc renderMode = NULL;
sprite* enemies = NULL;
sprite* friends = NULL;
sprite* neutrals = NULL;

int spriteMode(SDL_Renderer* renderer){
	renderSprites(neutrals,renderer);
    renderSprites(enemies,renderer);
    renderSprites(friends,renderer);
    return 0;
}

int setMode(renderFunc newMode){
	renderMode = newMode;
	return 0;
}

int render(SDL_Renderer* renderer){
	return renderMode(renderer);
}

