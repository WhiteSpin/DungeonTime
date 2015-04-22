#include "Level.h"
#include "greetings.h"
	

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
	Entity* entityAtPos = level->getEntityAt(posX, posY);
	if(entityAtPos) {
		LivingEntity* livingEntityAtPos = dynamic_cast<LivingEntity*>(entityAtPos);
		if(livingEntityAtPos) {
			if(livingEntityAtPos->friendly) {
				Message::push(get_random_greeting());
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
			//TODO deal with non-living Entities
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
}

bool LivingEntity::hurt(int damage) {
	health -= damage;
	//Message::push(std::to_string(health));
	if(health <= 0) {
		Message::push(inventory ? "True":"False");
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


ItemContainer::ItemContainer(Level* _level, uint64_t _posX, uint64_t _posY, std::unique_ptr<Inventory> _inventory)
	:Entity(_level, _posX, _posY) {
	inventory = std::move(_inventory);
}

void ItemContainer::doFrame() {
	//printf("%d",inventory->items.size());
	return;
	for(size_t i = 0; i < inventory->items.size(); ++i) {
		auto item = inventory->items[i].get();
		if(!item)
			continue;
		printf("%c", item->getApperance());
		break;
	}
}
