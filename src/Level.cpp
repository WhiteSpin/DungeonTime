#include "Level.h"

std::unique_ptr<Level> level;
LivingEntity* hero;

Level::Level() :width(75), height(25), background((uint8_t*)malloc(width * height)) {
	
}

Level::~Level() {

}


Room::Room(uint64_t _posX, uint64_t _posY, uint64_t _width, uint64_t _height):
	posX(_posX), posY(_posY), width(_width), height(_height) {
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
		} }
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
		if(getBackgroundAt(x, posY) != BACKGROUD_FLOOR)
			setBackgroundAt(x, posY, type);
		else
			setBackgroundAt(x, posY, BACKGROUD_FLOOR);

}

void Level::fillBackgroundColumn(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type) {
	for(uint64_t y = posY; y < posY+length; ++y)
		if(getBackgroundAt(posX, y)!= BACKGROUD_FLOOR)
			setBackgroundAt(posX, y, type);
		else
			setBackgroundAt(posX, y, BACKGROUD_FLOOR);
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
	rooms.push_back(std::unique_ptr<Room>(new Room(posX, posY, w, h)));
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
	generateRectRoom(width/2+5, height/2+5, 10, 10);
	memcpy(background.get()+(height/2-2)*width+(width/2), "Welcome Hero!", 13);
}

void Level::generateOther() {
	memset(background.get(), BACKGROUD_EMPTY, width * height);
	generateXSplitRoom(7, 7, width/4+1, height/4+1);
	generateYSplitRoom(width/2+10, height/2+10, width/4+1, height/4+1);
	memcpy(background.get()+(height/2-2)*width+(width/2), "Welcome Hero!", 13);
}

bool xOverlap(int x1, int x2, int x3, int x4) {
	return (x1 < x4 && x2 > x3);
}

bool yOverlap(int y1, int y2, int y3, int y4) {
	return (y1 < y4 && y2 > y3);
}

void Level::generateRandom() {
	memset(background.get(), BACKGROUD_EMPTY, width * height);
	srand(time(NULL));
	int roomNum = 30;
	int maxTries = 5000;
	while(roomNum>0) {
		if (maxTries == 0)
			break;
		maxTries--;
		bool collide = false;
		uint64_t w = 10 + (rand()+maxTries) % 12;
		maxTries--;
		uint64_t h = 5 + (rand()+maxTries) % 10;
		maxTries--;
		uint64_t posX = ((rand()+maxTries) % (width-w));	
		maxTries--;
		uint64_t posY = ((rand()+maxTries) % (height-h));	
		for(int i = 0; i < rooms.size(); ++i) {
			auto room = rooms[i].get();
			if(posX < room->posX+room->width+2 && posX+width+2 > room->posX &&
			posY < room->posY+room->height+2 && posY+height+2 > room->posY) {
				collide = true;
			}
		}
		if(!collide) {
			generateRectRoom(posX, posY, w, h);		
			roomNum--;
		}
	}
	
	if(rooms.size() >= 2) {
		for(int i = 0; i < rooms.size()-1;) {
			for(int j = i+1; j < rooms.size();) { 
				auto room1 = rooms[i].get();
				auto room2 = rooms[j].get();
				if(room1->posX < room2->posX+room2->width-3 && room1->posX+room1->width-3 > room2->posX) { //X-Overlap
					Message::push("Overlap");
					int Xstart = std::max(room1->posX,room2->posX);
					int Xstop = std::min(room1->posX+room1->width,room2->posX+room2->width);
					int Xmid = (Xstart+Xstop)/2;
					if(room1->posY+room1->height < room2->posY) {
						bool overlap = false;
						int corX = Xmid;
						int corY = room1->posY+room1->height-1;
						int corW = 3;
						int corH = room2->posY - (room1->posY+room1->height)+2;
						for(int k = 0; k < rooms.size(); ++k) {
							auto testedRoom = rooms[k].get();
							if (testedRoom != room1 && testedRoom != room2) {
								if(testedRoom->posX < corX+corW && testedRoom->posX+testedRoom->width > corX &&
									testedRoom->posY < corY+corH && testedRoom->posY+testedRoom->height > corY)
									overlap = true;
							}
						}
						if (!overlap)
							generateYCorridor(corX, corY, corW, corH);
					}
				}

				else {
					if(room1->posY < room2->posY+room2->height-4 && room1->posY+room1->height-4 > room2->posY) { //Y-Overlap
						Message::push("Overlap");
						int Ystart = std::max(room1->posY,room2->posY);
						int Ystop = std::min(room1->posY+room1->height,room2->posY+room2->height);
						int Ymid = (Ystart+Ystop)/2;
						if(room1->posX+room1->width < room2->posX) {
							bool overlap = false;
							int corX = room1->posX+room1->width-1;
							int corY = Ymid;
							int corW = room2->posX - (room1->posX+room1->width)+2;
							int corH = 3;
							for(int k = 0; k < rooms.size(); ++k) {
								auto testedRoom = rooms[k].get();
								if (testedRoom != room1 && testedRoom != room2) {
									if(testedRoom->posX < corX+corW && testedRoom->posX+testedRoom->width > corX &&
										testedRoom->posY < corY+corH && testedRoom->posY+testedRoom->height > corY)
										overlap = true;
								}
							}
							if(!overlap)
								generateXCorridor(corX, corY, corW, corH);
						}
					}
				}
			++j;
			}
		++i;
		}
	}
}

