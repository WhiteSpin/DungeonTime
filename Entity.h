#include "Message.h"

class Entity {
	public:
	enum Action {
		MoveUp = 1,
		MoveLeft = 2,
		MoveDown = 3,
		MoveRight = 4
	};

	uint64_t posX, posY;
	void doFrame();
	bool canEnter(uint64_t posX, uint64_t posY);
	bool handleAction(Action input);
};
