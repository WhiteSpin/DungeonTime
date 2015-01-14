#include "Map.h"

uint8_t moveKeyBinding[128];

int main(int argc, const char** argv) {
	Terminal::init();

	memset(moveKeyBinding, 0, sizeof(moveKeyBinding));
	moveKeyBinding['A'] = Up;
	moveKeyBinding['B'] = Down;
	moveKeyBinding['C'] = Right;
	moveKeyBinding['D'] = Left;
	moveKeyBinding['k'] = Up;
	moveKeyBinding['j'] = Down;
	moveKeyBinding['l'] = Right;
	moveKeyBinding['h'] = Left;

	map.generate();
	Entity hero;
	hero.posX = 17;
	hero.posY = 10;

	while(true) {
		uint8_t buffer[16], moveDir;
		uint64_t readBytes = Terminal::handleKeyboard(sizeof(buffer), buffer);
		if(readBytes == 3 && Terminal::isCSI(buffer))
			moveDir = moveKeyBinding[buffer[2]];
		else if(readBytes == 1)
			moveDir = moveKeyBinding[buffer[0]];
		if(readBytes && moveDir)
			hero.moveControl(moveDir);

		map.render();
		Terminal::setCursorPosition(hero.posX, hero.posY);
		fflush(stdout);

		usleep(50000);
		//for(int i = 0; i < n; i ++)
		//	printf("%d %x %d\n", i, buffer[i], buffer[i]);
	}

	return 0;
}
