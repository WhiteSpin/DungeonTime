#include "Map.h"

void Entity::doFrame() {
	printf("@");
}

bool Entity::canEnter(uint64_t posX, uint64_t posY) {
	return map.getBackgroundAt(posX, posY) == BACKGROUD_FLOOR;
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
		if(posY < map.height-1 && canEnter(posX, posY+1)) {
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
		if(posX < map.width-1 && canEnter(posX+1, posY)) {
			posX ++;
			return true;
		}
		return false;
	}
	Message::push(std::string("Not An Arrow to the knee"));
	return false;
}
