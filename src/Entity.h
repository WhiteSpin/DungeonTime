#include "Item.h"

class ItemContainer;

class Entity {
	public:
	Level* level;
	uint64_t posX, posY;
	std::unique_ptr<Inventory> inventory;
	Entity(Level* level, uint64_t posX, uint64_t posY, std::unique_ptr<Inventory> inventory = nullptr);
	virtual bool destroy();
	virtual void doFrame();
	bool tryToEnter(uint64_t posX, uint64_t posY);
	virtual bool handleAction(Controls::Action input);
};

class ItemContainer : public Entity {
	public:
	ItemContainer(Level* level, uint64_t posX, uint64_t posY, std::unique_ptr<Inventory> inventory);
	virtual void doFrame();
};

class MovingItemContainer : public ItemContainer {
	public:
	MovingItemContainer(Level* level, uint64_t posX, uint64_t posY, std::unique_ptr<Inventory> inventory);
	void land();
	std::vector<std::pair<uint64_t, uint64_t >> path;
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
	bool goTowards(uint16_t* field);
	void throwItem(std::unique_ptr<Inventory> inv, uint64_t targetX, uint64_t targetY);
};

