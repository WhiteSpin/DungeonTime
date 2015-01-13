#include "Map.h"

#define BACKGROUD_EMPTY ' '
#define BACKGROUD_FLOOR '.'
#define BACKGROUD_CORRIDOR '#'

Map::Map() :width(64), height(32), background((uint8_t*)malloc(width * height)) {

}

Map::~Map() {
	free(background);
}

void Map::render() {
	setCursorPosition(0, 0);
	for(uint64_t y = 0; y < height; y ++) {
		for(uint64_t x = 0; x < width; x ++) {
			fwrite(&background[y*width+x], 1, 1, stdout);
		}
		//fwrite(&background[y*width], 1, width, stdout);
		printf("\n");
	}
}

uint8_t Map::getBackgroundAt(uint64_t posX, uint64_t posY) {
	return background[posY*width+posX];
}

bool Map::replaceBackgroundCell(uint8_t type, uint64_t posX, uint64_t posY) {
	uint8_t* ptr = background+posY*width+posX;
	switch(*ptr) {
		case BACKGROUD_FLOOR:
		return false;
	}
	*ptr = type;
	return true;
}

void Map::fillBackgroundRow(uint8_t type, uint64_t posX, uint64_t posY, uint64_t length) {
	for(uint64_t x = posX; x < posX+length; x ++)
		replaceBackgroundCell(type, x, posY);
}

void Map::fillBackgroundColumn(uint8_t type, uint64_t posX, uint64_t posY, uint64_t length) {
	for(uint64_t y = posY; y < posY+length; y ++)
		replaceBackgroundCell(type, posX, y);
}

void Map::fillBackgroundRect(uint8_t type, uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	for(uint64_t y = posY; y < posY+h; y ++)
		for(uint64_t x = posX; x < posX+w; x ++)
			replaceBackgroundCell(type, x, y);
}

void Map::generateRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	fillBackgroundRow('-', posX+1, posY, w-2);
	fillBackgroundRow('-', posX+1, posY+h-1, w-2);
	fillBackgroundColumn('|', posX, posY+1, h-2);
	fillBackgroundColumn('|', posX+w-1, posY+1, h-2);
	fillBackgroundRect('.', posX+1, posY+1, w-2, h-2);
}

void Map::generateXSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	uint64_t half = w >> 1;
	generateRoom(posX, posY, half+1, h);
	generateRoom(posX+half, posY, w-half, h);
	replaceBackgroundCell(BACKGROUD_CORRIDOR, posX+half, posY+h/2);
}

void Map::generateYSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	uint64_t half = h >> 1;
	generateRoom(posX, posY, w, half+1);
	generateRoom(posX, posY+half, w, h-half);
	replaceBackgroundCell(BACKGROUD_CORRIDOR, posX+w/2, posY+half);
}

void Map::generateXCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	fillBackgroundRow(BACKGROUD_CORRIDOR, posX, posY, w);
	fillBackgroundRow(BACKGROUD_CORRIDOR, posX, posY+h-1, w);
	fillBackgroundRect(BACKGROUD_FLOOR, posX, posY+1, w, h-2);
}

void Map::generateYCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h) {
	fillBackgroundColumn(BACKGROUD_CORRIDOR, posX, posY, h);
	fillBackgroundColumn(BACKGROUD_CORRIDOR, posX+w-1, posY, h);
	fillBackgroundRect(BACKGROUD_FLOOR, posX+1, posY, w-2, h);
}

void Map::generate() {
	memset(background, BACKGROUD_EMPTY, width * height);
	generateXSplitRoom(8, 4, 15, 9);
	generateYSplitRoom(32, 4, 15, 9);
	generateXCorridor(22, 5, 11, 3);
	generateYCorridor(26, 3, 3, 11);
	//generateRoom(8, 2, 9, 4);
	//generateRoom(8, 2, 9, 4);
}

Map map;
