#include "Level.h"

float Item::getMass() const {
	return 1.0;
}

std::string Item::getDescription() const {
	return "Generic Item";
}



Inventory::Inventory(uint64_t slots) {
	for(uint64_t i = 0; i < slots; i ++)
		items.push_back(std::unique_ptr<Item>());
}

std::string Inventory::getDescription() const {
	return "Generic Inventory";
}

uint64_t Inventory::getSlotCount() const {
	return items.size();
}

std::string Inventory::getSlotName(uint64_t slot) const {
	return std::to_string(slot+1);
}

Item* Inventory::getItemInSlot(uint64_t slot) const {
	return items[slot].get();
}
