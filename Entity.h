#include "Message.h"

class Level;

class Entity {
	public:
	enum Action {
		MoveUp = 1,
		MoveLeft = 2,
		MoveDown = 3,
		MoveRight = 4
	};

	Level* level;
	uint64_t posX, posY;
	Entity(Level* level, uint64_t posX, uint64_t posY);
	bool die();
	void doFrame();
	bool canEnter(uint64_t posX, uint64_t posY);
	bool handleAction(Action input);
};
