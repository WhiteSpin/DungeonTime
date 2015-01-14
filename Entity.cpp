#include "Level.h"

Entity::Entity(Level* _level, uint64_t _posX, uint64_t _posY)
	:level(_level), posX(_posX), posY(_posY) {
		level->entities.push_back(std::unique_ptr<Entity>(this));
}

bool Entity::die() {
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

bool Entity::canEnter(uint64_t posX, uint64_t posY) {
	return level->getBackgroundAt(posX, posY) == BACKGROUD_FLOOR;
}

bool Entity::handleAction(Action input) {
	switch(input) {
		case MoveUp:
		if(posY > 0 && canEnter(posX, posY-1)) {
			posY --;
			return true;
		}
		return false;
		case MoveDown:
		if(posY < level->height-1 && canEnter(posX, posY+1)) {
			posY ++;
			return true;
		}
		return false;
		case MoveLeft:
		if(posX > 0 && canEnter(posX-1, posY)) {
			posX --;
			return true;
		}
		return false;
		case MoveRight:
		if(posX < level->width-1 && canEnter(posX+1, posY)) {
			posX ++;
			return true;
		}
		return false;
	}
	Message::push(std::string("Not An Arrow to the knee"));
	return false;
}
