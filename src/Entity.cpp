#include "Level.h"
#include "Greetings.h"


Entity::Entity(Level* _level, uint64_t _posX, uint64_t _posY)
	:level(_level), posX(_posX), posY(_posY) {
	level->entities.push_back(std::unique_ptr<Entity>(this));
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
		}else{
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
	}else{
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
		default:
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
	if(health <= 0) {
		if(inventory && inventory->getFilledSlotsCount() != 0)
			auto container = new ItemContainer(level, posX, posY, std::move(inventory));
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

std::vector<uint64_t> NOMIN = {USHRT_MAX, USHRT_MAX, USHRT_MAX};


std::vector<uint64_t> getMin(uint64_t _posX, uint64_t _posY, const std::vector<std::vector<uint64_t>> &set)  {
	std::vector<uint64_t> min = NOMIN;
	for(auto& i : set) {
		if(i[0] == _posX && i[1] == _posY) {
			if(i[2] < min[2]) {
				min = i;
			}
		}
	}
	return min;
}


std::vector<std::vector<uint64_t>> clear(std::vector<std::vector<uint64_t>> &set) {
	std::vector<std::vector<uint64_t>> cleanedSet;
	for(int x = 0; x<level->width; ++x) {
		for(int y = 0; y<level->height; ++y) {
			if(level->isWalkable(x, y) && x < level->width && y < level->height) {
				std::vector<uint64_t> min = getMin(x, y, set);
				if(min != NOMIN) {
					cleanedSet.push_back(min);
					level->setBackgroundAt(x, y, '0'+min[2]);
				}
				else
					level->setBackgroundAt(x, y, BACKGROUD_FLOOR);
			}
		}
	}
	System::writeToLog(std::to_string(set.size()) + "->" + std::to_string(cleanedSet.size()) + "\n");
	return cleanedSet;
}

void printSet(std::vector<std::vector<uint64_t>> &set) {
	for(auto& i : set) {
		Message::push("PosX: " + std::to_string(i[0]) + " PosY: " + std::to_string(i[1]) + " Dis: " + std::to_string(i[2]));
	}
}

void LivingEntity::goTowards(uint64_t _posX, uint64_t _posY) {
	std::vector<std::vector<uint64_t>> set;
	set.push_back({_posX, _posY, 0});
	int i = 0;
	while(true) {
		if(i < set.size()){
			auto cur = set[i];
			set.push_back({cur[0]+1, cur[1], cur[2]+1});
			set.push_back({cur[0], cur[1]+1, cur[2]+1});
			set.push_back({cur[0]-1, cur[1], cur[2]+1});
			set.push_back({cur[0], cur[1]-1, cur[2]+1});
			//System::writeToLog(std::to_string(set.size()));
			set = clear(set);
			i++;
		}
		else {
			checkSet(set);
			System::writeToLog("Break\n");
			break;
		}
	}
}

void LivingEntity::checkSet(std::vector<std::vector<uint64_t>> &set) {
	std::vector<uint64_t> min = NOMIN;

	for(auto& i : set) {
		if(i[0] == posX+1 && i[1] == posY && i[2] < min[2])
			min = i;
		if(i[0] == posX && i[1]	== posY+1 && i[2] < min[2])
			min = i;
		if(i[0] == posX-1 && i[1] == posY && i[2] < min[2])
			min = i;
		if(i[0] == posX && i[1] == posY-1 && i[2] < min[2])
			min = i;
	}
	if(min != NOMIN && level->isWalkable(min[0], min[1])) {
		posX = min[0];
		posY = min[1];
		Message::push("PosX: " + std::to_string(min[0]) + " PosY: " + std::to_string(min[1]) + " Dis: " + std::to_string(min[2]));
	}
}

ItemContainer::ItemContainer(Level* _level, uint64_t _posX, uint64_t _posY, std::unique_ptr<Inventory> _inventory)
	:Entity(_level, _posX, _posY) {
	inventory = std::move(_inventory);

	Entity* itemContainerAtPos = level->getItemContainerAt(posX, posY, this);
	if(itemContainerAtPos) {
		for(uint64_t i = 0; i < itemContainerAtPos->inventory->getSlotCount(); ++i) {
			auto itemInSlot = std::move(itemContainerAtPos->inventory->items[i]);
			if(itemInSlot)
				inventory->items.push_back(std::move(itemInSlot));
		}
		itemContainerAtPos->destroy();
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
}
