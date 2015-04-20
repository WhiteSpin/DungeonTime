#include "Level.h"

int main(int argc, const char** argv) {
	System::init();
	level.reset(new Level());
	hero = new Human(level.get(), level->width/2+1, level->height/2+1);
	hero->name = "The Chosen One";
	auto otherGuy = new LivingEntity(level.get(), level->width/2+2, level->height/2+1);
	otherGuy->name = "The Other One";
	otherGuy->friendly = true;

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
