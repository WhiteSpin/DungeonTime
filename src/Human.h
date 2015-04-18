#include "Entity.h"

class HumanInventory : public Inventory {
	public:
	HumanInventory();
	virtual std::string getDescription() const;
	virtual std::string getSlotName(uint64_t slot) const;
};

class Human : public LivingEntity {
	public:
	Human(Level* level, uint64_t posX, uint64_t posY);
};
