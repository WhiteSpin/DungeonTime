#include "Level.h"

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

int Entity::canEnter(uint64_t posX, uint64_t posY) {
	switch(level->getBackgroundAt(posX, posY)) {
		case BACKGROUD_FLOOR:
			return 1;
		case BACKGROUD_OPEN_DOOR:
			return 1;
		case BACKGROUD_CLOSED_DOOR:
			return 2;
	}
	return 0;
}

bool Entity::handleAction(Controls::Action input) {
	switch(input) {

		case Controls::Up:
		if(posY > 0) {
			int EnterStatus = canEnter(posX, posY-1);
			switch(EnterStatus) {
				case 1:
					posY --;
					return true;
				case 2:
					posY --;
					Message::push(std::string("The door opens"));
					return true;
				default:
					return false;
			}

		}
		else
			return false;

		case Controls::Down:
		if(posY < level->height-1) {
			int EnterStatus = canEnter(posX, posY+1);
			switch(EnterStatus) {
				case 1:
					posY ++;
					return true;
				case 2:
					posY ++;
					Message::push(std::string("The door opens"));
					return true;
				default:
					return false;
			}
		}
		else
			return false;

		case Controls::Left:
		if(posX > 0) {
			int EnterStatus = canEnter(posX-1, posY);
			switch(EnterStatus) {
				case 1:
					posX --;
					return true;
				case 2:
					posX --;
					Message::push(std::string("The door opens"));
					return true;
				default:
					return false;
			}
		}
		else
			return false;

		case Controls::Right:
		if(posX < level->width-1)  {
			int EnterStatus = canEnter(posX+1, posY);
			switch(EnterStatus) {
				case 1:
					posX ++;
					return true;
				case 2:
					posX ++;
					level->setBackgroundAt(posX, posY, BACKGROUD_OPEN_DOOR);
					Message::push(std::string("The door opens"));
					return true;
				default:
					return false;
			}
		}
		else
			return false;

		default:
		Message::push(std::string("Unknown command!"));
		return false;
	}
}

LivingEntity::LivingEntity(Level* _level, uint64_t _posX, uint64_t _posY)
	:Entity(_level, _posX, _posY) {
}

bool LivingEntity::destroy() {
	health = 0.0;
	return Entity::destroy();
}

bool LivingEntity::hurt(float damage) {
	health -= damage;
	if(health <= 0.0)
		return destroy();
	return false;
}

float LivingEntity::heal(float value) {
	if(health+value > maxHealth) {
		value = maxHealth-health;
		health = maxHealth;
		return value;
	}else{
		health += value;
		return 0.0;
	}
}

