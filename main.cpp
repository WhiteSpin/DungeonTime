#include "Map.h"

uint8_t moveKeyBinding[128];

int main(int argc, const char** argv) {
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
		uint64_t readBytes = term.handleKeyboard(sizeof(buffer), buffer);
		if(readBytes == 3 && term.isCSI(buffer))
			moveDir = moveKeyBinding[buffer[2]];
		else if(readBytes == 1)
			moveDir = moveKeyBinding[buffer[0]];
		if(readBytes){
			if(moveDir)
				hero.moveControl(moveDir);
			else {
				hero.keyControl(buffer[0]);
			}
		}

		map.render();
		printf("\33[J"); //Erase Display
		for(uint64_t i = 0; i < messages.size(); ++i) {
			messages[i]->render();
			if(messages[i]->lifeTime <= 0.0)
				messages.erase(messages.begin() + i--);
			// messages[i][0].render();
			// (*messages[i]).render();
		}

		term.setCursorPosition(hero.posX, hero.posY);
		fflush(stdout);

		usleep(50000);
		//for(int i = 0; i < n; i ++)
		//	printf("%d %x %d\n", i, buffer[i], buffer[i]);
	}

	return 0;
}
