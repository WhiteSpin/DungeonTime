#include "Item.h"

class Entity {
	public:
	Level* level;
	uint64_t posX, posY;
	std::unique_ptr<Inventory> inventory;
	Entity(Level* level, uint64_t posX, uint64_t posY);
	virtual bool destroy();
	virtual void doFrame();
	bool tryToEnter(uint64_t posX, uint64_t posY);
	virtual bool handleAction(Controls::Action input);
};

class LivingEntity : public Entity {
	public:
	int health, maxHealth;
	bool friendly;
	bool willTalk;
	std::string name;
	LivingEntity(Level* level, uint64_t posX, uint64_t posY);
	bool hurt(int damage);
	int heal(int value);
	void goTowards(uint64_t posX, uint64_t posY);
	void checkSet(std::vector<std::vector<uint64_t>> &set);
};

class ItemContainer : public Entity {
	public:
	ItemContainer(Level* level, uint64_t posX, uint64_t posY, std::unique_ptr<Inventory> inventory);
	virtual void doFrame();
};
