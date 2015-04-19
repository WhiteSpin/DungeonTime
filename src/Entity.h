#include "Item.h"

class Entity {
	public:
	Level* level;
	uint64_t posX, posY;
	std::unique_ptr<Inventory> inventory;

	Entity(Level* level, uint64_t posX, uint64_t posY);
	virtual bool destroy();
	void doFrame();
	int canEnter(uint64_t posX, uint64_t posY);
	virtual bool handleAction(Controls::Action input);
};

class LivingEntity : public Entity {
	public:
	Level* level;
	uint64_t posX, posY;
	float health, maxHealth;
	std::unique_ptr<Inventory> inventory;
	LivingEntity(Level* level, uint64_t posX, uint64_t posY);
	virtual bool destroy();
	bool hurt(float damage);
	float heal(float value);
};
