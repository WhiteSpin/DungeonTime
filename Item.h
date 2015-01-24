#include "Controls.h"

class Inventory;

class Item {
	public:
	float getMass() const;
	virtual std::string getDescription() const;
};

class Inventory : public Item {
	public:
	std::vector<std::unique_ptr<Item>> items;
	Inventory(uint64_t slots);
	virtual std::string getDescription() const;
	virtual uint64_t getSlotCount() const;
	virtual std::string getSlotName(uint64_t slot) const;
	Item* getItemInSlot(uint64_t slot) const;
};
