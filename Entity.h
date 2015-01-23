#include "Item.h"

class Entity {
	public:
	Level* level;
	uint64_t posX, posY;
	float health, maxHealth;
	std::unique_ptr<Inventory> inventory;

	Entity(Level* level, uint64_t posX, uint64_t posY);
	bool die();
	bool hurt(float damage);
	float heal(float value);
	void doFrame();
	bool canEnter(uint64_t posX, uint64_t posY);
	bool handleAction(Controls::Action input);
};
