#include "Controls.h"

class Level;

class Entity {
	public:
	float health, maxHealth;
	Level* level;
	uint64_t posX, posY;
	Entity(Level* level, uint64_t posX, uint64_t posY);
	bool die();
	bool hurt(float damage);
	float heal(float value);
	void doFrame();
	bool canEnter(uint64_t posX, uint64_t posY);
	bool handleAction(Controls::Action input);
};
