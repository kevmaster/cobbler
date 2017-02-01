#include "engine.h"

#include "stdlib.h"


int init_engine(
{
	if(engine != NULL) return -1;

	engine = (*Engine) malloc(sizeof(Engine));
	if(engine == NULL) return -1;
}

void destroy_list_helper(Particle* head)
{
	if(head == NULL) return;
	destroy_list_helper(head->next);
	free(head);
	engine = NULL;
}

int destruct_engine(void)
{
	destroy_list_helper(engine->head);
	free(engine);
}

int add_particle(Particle* p)
{
	Particle* temp = engine->p_list;
	if(temp == NULL){
		p_list = p;
	}
	else{
		p_tail->next = p;
	}
	p->prev = p_tail;
	p_tail = p;
	p->next = NULL;
}

int update_engine(void){
	if(!engine){
		return -1;
	}
	Particle* temp = engine->p_list;
	while(temp != NULL)
	{
		//update physics things
		temp = temp->next;
	}
	return 0;
}
