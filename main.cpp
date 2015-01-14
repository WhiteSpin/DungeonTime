#include "Map.h"

uint8_t moveKeyBinding[128];

int main(int argc, const char** argv) {
	System::init();

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
		uint64_t readBytes = System::handleKeyboard(sizeof(buffer), buffer);
		if(readBytes == 3 && System::isCSI(buffer))
			moveDir = moveKeyBinding[buffer[2]];
		else if(readBytes == 1)
			moveDir = moveKeyBinding[buffer[0]];
		if(readBytes){
			if(moveDir)
				hero.moveControl(moveDir);
			else
				hero.keyControl(buffer[0]);
		}

		System::doFrame();
		map.doFrame();
		System::eraseStartingAtLine(map.height);
		for(auto i = messages.begin(); i != messages.end(); ) {
			Message* msg = (*i).get();
			msg->doFrame();
			if(msg->lifeTime > 0.0)
				++i;
			else
				messages.erase(i);
		}
		System::setCursorPosition(hero.posX, hero.posY);

		/*buffer[readBytes] = 0;
		printf("%s\n", buffer);
		for(int i = 0; i < readBytes; i ++)
			printf("%d %x %d\n", i, buffer[i], buffer[i]);*/
	}

	return 0;
}
