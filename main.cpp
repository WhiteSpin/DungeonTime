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
		System::setCursorPosition(hero->posX, hero->posY);
	}

	return 0;
}
