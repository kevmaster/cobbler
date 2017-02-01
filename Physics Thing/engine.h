#include "item.h"

typedef engine {
	Particle* p_list;
	Particle* p_tail;
} Engine;

Engine* engine;

int init_engine(void);
int destruct_engine(void);
int update_engine(void);
int add_particle(Particle* p);