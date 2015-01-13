#include "Map.h"

void Entity::render() {
	printf("@");
}

bool Entity::canEnter(uint64_t posX, uint64_t posY) {
	return map.getBackgroundAt(posX, posY) == BACKGROUD_FLOOR;
}

bool Entity::handleControl(uint8_t input) {
	switch(input) {
		case 65:
		if(posY > 0 && canEnter(posX, posY-1)) {
			posY --;
			return true;
		}
		return false;
		case 66:
		if(posY < map.height-1 && canEnter(posX, posY+1)) {
			posY ++;
			return true;
		}
		return false;
		case 67:
		if(posX < map.width-1 && canEnter(posX+1, posY)) {
			posX ++;
			return true;
		}
		return false;
		case 68:
		if(posX > 0 && canEnter(posX-1, posY)) {
			posX --;
			return true;
		}
		return false;
	}
	return false;
}
