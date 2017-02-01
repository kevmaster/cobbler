typedef struct V3_T {
	double x,y,z;
} Vector3;

typedef struct T {
	struct T* next;
	struct T* prev;
	double mass;
	Vector3 position;
	Vector3 velocity;
	Force* force_list;

} Particle;

typedef struct Force_T {
	Vector3 direction;
	struct Force_T* next;
} Force;