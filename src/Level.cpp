#include "Level.h"

std::unique_ptr<Level> level;
LivingEntity* hero;

Level::Level() :width(75), height(25), background((uint8_t*)malloc(width * height)) {
	generate();
}

Level::~Level() {

}

void Level::doFrame() {
	uint64_t maxX = std::min(width, (uint64_t)System::screenSize.ws_col),
			 maxY = std::min(height, (uint64_t)System::screenSize.ws_row);
	for(uint64_t y = 0; y < maxY; ++y) {
		System::setCursorPosition(0, y);
		for(uint64_t x = 0; x < maxX; ++x) {
			auto entity = getPrimeEntityAt(x, y);
			if(entity)
				entity->doFrame();
			else
				fwrite(background.get()+y*width+x, 1, 1, stdout);
		}
	}
}

Entity* Level::getPrimeEntityAt(uint64_t posX, uint64_t posY) const {
	for(uint64_t i = 0; i < entities.size(); ++i)
		if(entities[i]->posX == posX && entities[i]->posY == posY) {
			Entity* LivingEntityAtPos = dynamic_cast<LivingEntity*>(entities[i].get());
			if(LivingEntityAtPos)
				return LivingEntityAtPos;
		}
				
	return getItemContainerAt(posX, posY);
}

LivingEntity* Level::getLivingEntityAt(uint64_t posX, uint64_t posY, Entity* exceptFrom) const {
	for(uint64_t i = 0; i < entities.size(); ++i)
		if(entities[i]->posX == posX && entities[i]->posY == posY) {
			auto livingEntityAtPos = dynamic_cast<LivingEntity*>(entities[i].get());
			if (livingEntityAtPos && livingEntityAtPos != exceptFrom)
				return livingEntityAtPos;
		}
	return NULL;
}

ItemContainer* Level::getItemContainerAt(uint64_t posX, uint64_t posY, Entity* exceptFrom) const {
	for(uint64_t i = 0; i < entities.size(); ++i)
		if(entities[i]->posX == posX && entities[i]->posY == posY) {
			auto itemContainerAtPos = dynamic_cast<ItemContainer*>(entities[i].get());
			if (itemContainerAtPos && itemContainerAtPos != exceptFrom)
				return itemContainerAtPos;
		}
	return NULL;
}

uint8_t Level::getBackgroundAt(uint64_t posX, uint64_t posY) const {
	return *(background.get()+posY*width+posX);
}

void Level::setBackgroundAt(uint64_t posX, uint64_t posY, uint8_t type) {
	*(background.get()+posY*width+posX) = type;
}

void Level::fillBackgroundRow(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type) {
	for(uint64_t x = posX; x < posX+length; ++x)
		setBackgroundAt(x, posY, type);
}

void Level::fillBackgroundColumn(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type) {
	for(uint64_t y = posY; y < posY+length; ++y)
		setBackgroundAt(posX, y, type);
}

void Level::fillBackgroundRect(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h, uint8_t type) {
	for(uint64_t y = posY; y < posY+h; ++y)
		for(uint64_t x = posX; x < posX+w; ++x)
			setBackgroundAt(x, y, type);
}

void Level::generateLine(uint64_t fromX, uint64_t fromY, uint64_t toX, uint64_t toY, uint8_t type) {
	//Bresenham Line
	int dx =  abs(toX-fromX), sx = fromX<toX ? 1 : -1;
	int dy = -abs(toY-fromY), sy = fromY<toY ? 1 : -1;
	int err = dx+dy, e2;

	while(true) {
		setBackgroundAt(fromX, fromY, type);
		if(fromX == toX && fromY == toY)
			break;

		e2 = 2*err;
		if(e2 > dy) { err += dy; fromX += sx; }
		if(e2 < dx) { err += dx; fromY += sy; }
	}
}

void Level::generateEllipseRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	//Bresenham Ellipse

	int64_t dx = 0, dy = h, w2 = w*w, h2 = h*h, err = h2-(2*h-1)*w2, e2;

	do {
		setBackgroundAt(posX-dx, posY-dy, BACKGROUD_CORRIDOR);
		fillBackgroundColumn(posX-dx, posY-dy+1, dy*2-1, BACKGROUD_FLOOR);
		setBackgroundAt(posX-dx, posY+dy, BACKGROUD_CORRIDOR);

		setBackgroundAt(posX+dx, posY-dy, BACKGROUD_CORRIDOR);
		fillBackgroundColumn(posX+dx, posY-dy+1, dy*2-1, BACKGROUD_FLOOR);
		setBackgroundAt(posX+dx, posY+dy, BACKGROUD_CORRIDOR);

		e2 = 2*err;
		if(e2 <  (2*dx+1)*h2) err += (2*(++dx)+1)*h2;
		if(e2 > -(2*dy-1)*w2) err -= (2*(--dy)-1)*w2;
	} while(dy >= 0);

	while(dx++ < w) {
		setBackgroundAt(posX-dx, posY, BACKGROUD_CORRIDOR);
		setBackgroundAt(posX+dx, posY, BACKGROUD_CORRIDOR);
	}
}

void Level::generateRectRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	fillBackgroundRow(posX+1, posY, w-2, BACKGROUD_WALLX);
	fillBackgroundRow(posX+1, posY+h-1, w-2, BACKGROUD_WALLX);
	fillBackgroundColumn(posX, posY+1, h-2, BACKGROUD_WALLY);
	fillBackgroundColumn(posX+w-1, posY+1, h-2, BACKGROUD_WALLY);
	fillBackgroundRect(posX+1, posY+1, w-2, h-2, BACKGROUD_FLOOR);
}

void Level::generateXSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	uint64_t half = w >> 1;
	generateRectRoom(posX, posY, half+1, h);
	generateRectRoom(posX+half, posY, w-half, h);
	setBackgroundAt(posX+half, posY+h/2, BACKGROUD_CLOSED_DOOR);
}

void Level::generateYSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	uint64_t half = h >> 1;
	generateRectRoom(posX, posY, w, half+1);
	generateRectRoom(posX, posY+half, w, h-half);
	setBackgroundAt(posX+w/2, posY+half, BACKGROUD_CLOSED_DOOR);
}

void Level::generateXCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	fillBackgroundRow(posX, posY, w, BACKGROUD_CORRIDOR);
	fillBackgroundRow(posX, posY+h-1, w, BACKGROUD_CORRIDOR);
	fillBackgroundRect(posX, posY+1, w, h-2, BACKGROUD_FLOOR);
}

void Level::generateYCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	fillBackgroundColumn(posX, posY, h, BACKGROUD_CORRIDOR);
	fillBackgroundColumn(posX+w-1, posY, h, BACKGROUD_CORRIDOR);
	fillBackgroundRect(posX+1, posY, w-2, h, BACKGROUD_FLOOR);
}

void Level::generate() {
	memset(background.get(), BACKGROUD_EMPTY, width * height);
	generateXSplitRoom(width/2, height/2, width/4+1, height/4+1);
	memcpy(background.get()+(height/2-2)*width+(width/2), "Welcome Hero!", 13);
}
