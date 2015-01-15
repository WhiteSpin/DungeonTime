#include "Level.h"

int main(int argc, const char** argv) {
	System::init();
	level.reset(new Level());
	hero = new Entity(level.get(), 10, 5);
	Controls::init();

	while(true) {
		System::doFrame();
		Controls::doFrame();
		level->doFrame();
		System::eraseStartingAtLine(level->height);
		Message::doFrame();
		System::renderRightAlignedText(0, "Inventory");
		System::renderRightAlignedText(1, "a : Apply");
		System::renderRightAlignedText(2, "d : Drop");
		System::renderRightAlignedText(3, "s : Swap");

		System::setCursorPosition(hero->posX, hero->posY);
	}

	return 0;
}
