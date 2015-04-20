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

float Weapon::getDamage() const {
	const float damages[] = {
		2.0,5.0,8.0
	};
	return damages[type];
}
