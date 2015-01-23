#include "Level.h"

float Item::getMass() const {
	return 1.0;
}

std::string Item::getDescription() const {
	return "An Item";
}



Inventory::Inventory(uint64_t slots) {
	for(uint64_t i = 0; i < slots; i ++)
		items.push_back(std::unique_ptr<Item>());
}

Item* Inventory::getItemInSlot(uint64_t slot) const {
	return items[slot].get();
}

const std::string Inventory::getSlotDescription(uint64_t slot) const {
	std::stringstream ss;
	ss << getSlotName(slot) << " : " << getItemInSlot(slot)->getDescription();
	return ss.str();
}

const std::string Inventory::getSlotName(uint64_t slot) const {
	return std::to_string(slot+1);
}
