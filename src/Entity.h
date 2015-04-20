#include "Item.h"

class Entity {
	public:
	Level* level;
	uint64_t posX, posY;
	std::unique_ptr<Inventory> inventory;
	Entity(Level* level, uint64_t posX, uint64_t posY);
	virtual bool destroy();
	void doFrame();
	bool tryToEnter(uint64_t posX, uint64_t posY);
	virtual bool handleAction(Controls::Action input);
};

class LivingEntity : public Entity {
	public:
	Level* level;
	uint64_t posX, posY;
	float health, maxHealth;
	bool friendly;
	std::string name;
	LivingEntity(Level* level, uint64_t posX, uint64_t posY);
	bool hurt(float damage);
	float heal(float value);
};
