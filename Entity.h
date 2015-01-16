#include "Item.h"

class Entity {
	public:
	float health, maxHealth;
	std::vector<std::unique_ptr<Item>> inventory;
	Level* level;
	uint64_t posX, posY;
	Entity(Level* level, uint64_t posX, uint64_t posY);
	virtual const char* getInventorySlotName(uint64_t index) const;
	bool die();
	bool hurt(float damage);
	float heal(float value);
	void doFrame();
	bool canEnter(uint64_t posX, uint64_t posY);
	bool handleAction(Controls::Action input);
};
