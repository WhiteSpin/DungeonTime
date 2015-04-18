#include "Level.h"

int main(int argc, const char** argv) {
	System::init();
	level.reset(new Level());
	hero = new Human(level.get(), level->width/2+1, level->height/2+1);
	Controls::init();

	while(true) {
		System::doFrame();
		Controls::doFrame();
		level->doFrame();
		Message::doFrame();
		System::setCursorPosition(hero->posX, hero->posY);
	}

	return 0;
}
