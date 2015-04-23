#include "Level.h"

Inventory::Inventory(uint64_t slots) {
	for(uint64_t i = 0; i < slots; i ++)
		items.push_back(std::unique_ptr<Item>());
}

uint8_t Inventory::getApperance() const {
	return 0;
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

void Inventory::setItemInSlot(Item* item, uint64_t slot) {
	items[slot].reset(item);
}

Item* Inventory::getItemInSlot(uint64_t slot) const {
	return items[slot].get();
}

uint64_t Inventory::getFilledSlotsCount() const {
	int count = 0;
	for(size_t i = 0; i < items.size(); ++i) {
		auto item = items[i].get();
		if(item)
			count++;
	}
	return count;
}

void Inventory::printFilledSlots() const {
	for(size_t i = 0; i < items.size(); ++i) {
		auto item = items[i].get();
		if(item)
			Message::push("Item in inv: " + item->getDescription());
	}
	return;
}

Weapon::Weapon(WeaponType _type) :type(_type) {
}

std::string Weapon::getDescription() const {
	const char* descriptions[] = {
		"Bow","Sword","Axe"	
	};
	return descriptions[type];
}

uint8_t Weapon::getApperance() const {
	const uint8_t apperances[] = {
		'(','/','p'
	};
	return apperances[type];
}

int Weapon::getDamage() const {
	const int damages[] = {
		2,5,8
	};
	return damages[type];
}
