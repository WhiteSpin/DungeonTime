#include "Controls.h"

class Inventory;

class Item {
	public:
	virtual std::string getDescription() const = 0;
	virtual uint8_t getApperance() const = 0;
};

class Inventory : public Item {
	public:
	std::vector<std::unique_ptr<Item>> items;
	Inventory();
	Inventory(uint64_t slots);
	virtual uint8_t getApperance() const;
	virtual std::string getDescription() const;
	virtual uint64_t getSlotCount() const;
	virtual std::string getSlotName(uint64_t slot) const;
	void setItemInSlot(Item* item, uint64_t slot);
	Item* getItemInSlot(uint64_t slot) const;
};

class Weapon : public Item {
	public:
	enum WeaponType {
		Bow,
		Sword,
		Axe		
	} type;
	Weapon(WeaponType type);
	virtual std::string getDescription() const;
	virtual uint8_t getApperance() const;
	virtual int getDamage() const;
};
