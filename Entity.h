#include "Terminal.h"

enum Direction {
		Up = 1,
		Left = 2,
		Down = 3,
		Right = 4
};

class Entity {
	public:
	uint64_t posX, posY;
	void render();
	bool canEnter(uint64_t posX, uint64_t posY);
	bool moveControl(uint8_t input);
};
