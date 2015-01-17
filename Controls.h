#include "Message.h"

namespace Controls {
	enum Action {
		NotAssigned = 0,
		Up,
		Down,
		Left,
		Right,
		Delete,
		Cancel,
		Confirm,
		Primary,
		Secondary,
		SelectSlot
	};

	enum Mode {
		Move,
		Command,
		ItemSelection
	};

	void init();
	bool tryToSelectItemSlot(Action action);
	void printItemSlot(char* buffer, uint64_t index);
	void handleInput(uint8_t* input, Action action);
	void doFrame();
};
