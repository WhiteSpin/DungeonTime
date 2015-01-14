#include "Level.h"

Entity::Entity(Level* _level, uint64_t _posX, uint64_t _posY)
	:level(_level), posX(_posX), posY(_posY) {
		level->entities.push_back(std::unique_ptr<Entity>(this));
}

bool Entity::die() {
	health = 0.0;
	for(uint64_t i = 0; i < level->entities.size(); ++i)
		if(level->entities[i].get() == this) {
			level->entities.erase(level->entities.begin()+i);
			return true;
		}
	return false;
}

bool Entity::hurt(float damage) {
	health -= damage;
	if(health <= 0.0)
		return die();
	return false;
}

float Entity::heal(float value) {
	if(health+value > maxHealth) {
		value = maxHealth-health;
		health = maxHealth;
		return value;
	}else{
		health += value;
		return 0.0;
	}
}

void Entity::doFrame() {
	printf("@");
}

bool Entity::canEnter(uint64_t posX, uint64_t posY) {
	return level->getBackgroundAt(posX, posY) == BACKGROUD_FLOOR;
}

bool Entity::handleAction(Controls::Action input) {
	switch(input) {
		case Controls::Up:
		if(posY > 0 && canEnter(posX, posY-1)) {
			posY --;
			return true;
		}
		return false;
		case Controls::Down:
		if(posY < level->height-1 && canEnter(posX, posY+1)) {
			posY ++;
			return true;
		}
		return false;
		case Controls::Left:
		if(posX > 0 && canEnter(posX-1, posY)) {
			posX --;
			return true;
		}
		return false;
		case Controls::Right:
		if(posX < level->width-1 && canEnter(posX+1, posY)) {
			posX ++;
			return true;
		}
		return false;
		default:
		Message::push(std::string("Unknown command!"));
		return false;
	}
}
