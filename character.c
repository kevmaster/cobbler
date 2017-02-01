#include "character.h"

Protag* createProtagonist(){
	Protag* out = (Protag*) malloc(sizeof(Protag));
	out->health = 100;
	out->moveSpeed = 150;
	out->image = (sprite*)
}

void destroyProtagonist(Protag* protag){

}

void moveProtag(Protag* protag, DIRECTION direction){

}

void unmoveProtag(Protag* protag, DIRECTION direction){

}