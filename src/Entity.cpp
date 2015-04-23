#include "Level.h"
#include "greetings.h"
	

Entity::Entity(Level* _level, uint64_t _posX, uint64_t _posY)
	:level(_level), posX(_posX), posY(_posY) {
	level->entities.push_back(std::unique_ptr<Entity>(this));
	inventory.reset(new Inventory);
}

bool Entity::destroy() {
	for(uint64_t i = 0; i < level->entities.size(); ++i)
		if(level->entities[i].get() == this) {
			level->entities.erase(level->entities.begin()+i);
			return true;
		}
	return false;
}

void Entity::doFrame() {
	printf("@");
}

bool Entity::tryToEnter(uint64_t posX, uint64_t posY) {
	LivingEntity* livingEntityAtPos = level->getLivingEntityAt(posX, posY);
	if(livingEntityAtPos) {
		if(livingEntityAtPos->friendly) {
			if(livingEntityAtPos->willTalk) {
				Message::push(get_random_greeting());
				livingEntityAtPos->willTalk = false;
			}
			return false;
		}
		else {
			Message::push(std::string("You hit ") + (livingEntityAtPos->name));
			Weapon* weapon = dynamic_cast<Weapon*>(inventory->getItemInSlot(0));
			if(weapon) {
				Message::push(livingEntityAtPos->name + " -" + std::to_string(weapon->getDamage()));
				if(livingEntityAtPos->hurt(weapon->getDamage())) {
					Message::push("You kill " + livingEntityAtPos->name);
				}
			}
			return false;
		}
	}
	else {
		ItemContainer* ItemContainerAtPos = level->getItemContainerAt(posX, posY);
		if(ItemContainerAtPos && ItemContainerAtPos->inventory->getFilledSlotsCount() > 0) {
			ItemContainerAtPos->inventory->printFilledSlots();
		}
	}
	
	switch(level->getBackgroundAt(posX, posY)) {
		case BACKGROUD_CLOSED_DOOR:
			level->setBackgroundAt(posX, posY, BACKGROUD_OPEN_DOOR);
			Message::push(std::string("The door opens"));
		case BACKGROUD_FLOOR:
		case BACKGROUD_OPEN_DOOR:
			return true;
	}
	return false;
}

bool Entity::handleAction(Controls::Action input) {
	switch(input) {
		case Controls::Up:
			if(posY > 0 && tryToEnter(posX, posY-1))
				posY --;
			return true;
		case Controls::Down:
			if(posY < level->height-1 && tryToEnter(posX, posY+1))
				posY ++;
			return true;
		case Controls::Left:
			if(posX > 0 && tryToEnter(posX-1, posY))
				posX --;
			return true;
		case Controls::Right:
			if(posX < level->width-1 && tryToEnter(posX+1, posY))
				posX ++;
			return true;
		default:
			Message::push(std::string("Unknown command!"));
			return false;
	}
}

LivingEntity::LivingEntity(Level* _level, uint64_t _posX, uint64_t _posY)
	:Entity(_level, _posX, _posY), maxHealth(12.0) {
	health = maxHealth;
	willTalk = true;
}

bool LivingEntity::hurt(int damage) {
	health -= damage;
	//Message::push(std::to_string(health));
	if(health <= 0) {
		if (inventory->getFilledSlotsCount() != 0) {
			Message::push("Passing on");
			auto container = new ItemContainer(level, posX, posY, std::move(inventory));
		}
		return destroy();
	}
	return false;
}

int LivingEntity::heal(int value) {
	if(health+value > maxHealth) {
		value = maxHealth-health;
		health = maxHealth;
		return value;
	}else{
		health += value;
		return 0;
	}
}


ItemContainer::ItemContainer(Level* _level, uint64_t _posX, uint64_t _posY, std::unique_ptr<Inventory> _inventory)
	:Entity(_level, _posX, _posY) {
	Entity* ItemContainerAtPos = level->getItemContainerAt(posX, posY);
	if (ItemContainerAtPos && ItemContainerAtPos->inventory->getFilledSlotsCount()!= 0) {
		auto mergedInventory = std::unique_ptr<Inventory>(new Inventory(_inventory->getFilledSlotsCount()+ItemContainerAtPos->inventory->getFilledSlotsCount()));
		int mergedInventoryPos = 0;
		for(int i = 0; i < _inventory->getSlotCount(); ++i) {
			Item* itemInSlot = _inventory->getItemInSlot(i);
			if (itemInSlot) {
				mergedInventory->setItemInSlot(std::move(itemInSlot),mergedInventoryPos);
				mergedInventoryPos++;	
			}
		}
		Message::push("1:");
		mergedInventory->printFilledSlots();
		for(int i = 0; i < ItemContainerAtPos->inventory->getSlotCount(); ++i) {
			Item* itemInSlot = ItemContainerAtPos->inventory->getItemInSlot(i);
			if (itemInSlot) {
				mergedInventory->setItemInSlot(std::move(itemInSlot),mergedInventoryPos);
				mergedInventoryPos++;	
			}
		}
		
		for(uint64_t i = 0; i < level->entities.size(); ++i) {
			if(level->entities[i].get() == ItemContainerAtPos) {
				//level->entities.erase(level->entities.begin()+i);
				Message::push("Now they come");
				(*(level->entities.begin()+i))->inventory->printFilledSlots();
			}
		}
		//ItemContainerAtPos->inventory.reset();
		ItemContainerAtPos->destroy();
		Message::push("2:");
		mergedInventory->printFilledSlots();
		inventory = std::move(mergedInventory);
	}
	else {
		inventory = std::move(_inventory);
	}
}

void ItemContainer::doFrame() {
	for(size_t i = 0; i < inventory->items.size(); ++i) {
		auto item = inventory->items[i].get();
		if(!item)
			continue;
		printf("%c", item->getApperance());
		return;
	}
	//printf("%c",level->getBackgroundAt(posX,posY));
	//destroy();
   	return;
}
