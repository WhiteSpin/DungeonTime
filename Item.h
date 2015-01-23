#include "Controls.h"

class Item {
	public:
	float getMass() const;
	std::string getDescription() const;
};

class Inventory {
	public:
	std::vector<std::unique_ptr<Item>> items;
	Inventory(uint64_t slots);
	Item* getItemInSlot(uint64_t slot) const;
	const std::string getSlotDescription(uint64_t slot) const;
	virtual const std::string getSlotName(uint64_t slot) const;
};
