#include "Controls.h"

int main(int argc, const char** argv) {
	System::init();
	level.reset(new Level());
	Controls::init();

	while(true) {
		System::doFrame();
		Controls::doFrame();
		level->doFrame();
		System::eraseStartingAtLine(level->height);
		Message::doFrame();
		System::setCursorPosition(hero->posX, hero->posY);

		/*buffer[readBytes] = 0;
		printf("%s\n", buffer);
		for(int i = 0; i < readBytes; i ++)
			printf("%d %x %d\n", i, buffer[i], buffer[i]);*/
	}

	return 0;
}
