#include "Message.h"

namespace Controls {
	enum Action {
		NotAssigned = 0,
		Up,
		Down,
		Left,
		Right,
		Throw,
		Door,
		God,
		Delete,
		Cancel,
		Confirm,
		Primary,
		Secondary,
		Select0,
		Select9 = Select0+9
	};

	enum Mode {
		Move,
		Command
	};

	void init();
	Item* printItemSlot(char* buffer, uint64_t index);
	uint64_t handleInput(bool special, uint64_t size, const char* buffer);
	void doFrame();
};
