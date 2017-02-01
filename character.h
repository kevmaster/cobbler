#include "sprites.h"
enum DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};
typedef struct Protag_T {
	sprite* image;
	int health;
	int moveSpeed;
} Protag;

Protag* createProtagonist();

void destroyProtagonist(Protag* protag);

void moveProtag(Protag* protag, DIRECTION direction);

void unmoveProtag(Protag* protag, DIRECTION direction);