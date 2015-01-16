#include "Level.h"

const char* inventorySlotNames[] = {
	"Right Hand",
	"Left Hand",
	"Back",
	"Waist",
	"Head",
	"Hands",
	"Chest",
	"Legs",
	"Feet"
};

Human::Human(Level* _level, uint64_t _posX, uint64_t _posY) :Entity(_level, _posX, _posY) {
	for(uint64_t i = 0; i < 9; i ++)
		inventory.push_back(std::unique_ptr<Item>());
}

const char* Human::getInventorySlotName(uint64_t index) const {
	return inventorySlotNames[index];
}
