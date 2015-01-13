#include "Terminal.h"

class Entity {
	public:
	uint64_t posX, posY;
	void render();
	bool canEnter(uint64_t posX, uint64_t posY);
	bool handleControl(uint8_t input);
};
