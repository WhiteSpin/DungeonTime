#include "Message.h"

namespace Controls {
	enum Action {
		NotAssigned = 0,
		Pause,
		Up,
		Down,
		Left,
		Right,
		Apply,
		Drop,
		Swap,
		SelectSlot
	};

	void init();
	void doFrame();
};
