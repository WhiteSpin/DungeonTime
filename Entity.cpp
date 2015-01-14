#include "Map.h"

void Entity::doFrame() {
	printf("@");
}

bool Entity::canEnter(uint64_t posX, uint64_t posY) {
	return map.getBackgroundAt(posX, posY) == BACKGROUD_FLOOR;
}

bool Entity::moveControl(uint8_t input) {
	switch(input) {
		case Up:
		if(posY > 0 && canEnter(posX, posY-1)) {
			posY --;
			return true;
		}
		return false;
		case Down:
		if(posY < map.height-1 && canEnter(posX, posY+1)) {
			posY ++;
			return true;
		}
		return false;
		case Left:
		if(posX > 0 && canEnter(posX-1, posY)) {
			posX --;
			return true;
		}
		return false;
		case Right:
		if(posX < map.width-1 && canEnter(posX+1, posY)) {
			posX ++;
			return true;
		}
		return false;
	}
	return false;
}

bool Entity::keyControl(uint8_t input) {
	messages.push_back(std::unique_ptr<Message>(new Message("Not An Arrow to the knee")));
	return false;
}
