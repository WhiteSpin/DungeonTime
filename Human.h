#include "Entity.h"

class Human : public Entity {
	public:
	Human(Level* level, uint64_t posX, uint64_t posY);
	const char* getInventorySlotName(uint64_t index) const;
};
