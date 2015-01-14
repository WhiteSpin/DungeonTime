#include "Map.h"

uint8_t asciiKeyBinding[128];
uint8_t ansiKeyBinding[128];

int main(int argc, const char** argv) {
	System::init();

	memset(asciiKeyBinding, 0, sizeof(asciiKeyBinding));
	asciiKeyBinding['k'] = Entity::MoveUp;
	asciiKeyBinding['j'] = Entity::MoveDown;
	asciiKeyBinding['l'] = Entity::MoveRight;
	asciiKeyBinding['h'] = Entity::MoveLeft;

	memset(ansiKeyBinding, 0, sizeof(ansiKeyBinding));
	ansiKeyBinding['A'] = Entity::MoveUp;
	ansiKeyBinding['B'] = Entity::MoveDown;
	ansiKeyBinding['C'] = Entity::MoveRight;
	ansiKeyBinding['D'] = Entity::MoveLeft;

	map.generate();
	Entity hero;
	hero.posX = 17;
	hero.posY = 10;

	while(true) {
		Entity::Action action;
		uint8_t buffer[16];
		uint64_t readBytes = System::handleKeyboard(sizeof(buffer), buffer);
		if(readBytes == 3 && System::isCSI(buffer))
			action = (Entity::Action)ansiKeyBinding[buffer[2]];
		else if(readBytes == 1)
			action = (Entity::Action)asciiKeyBinding[buffer[0]];
		if(readBytes)
			hero.handleAction(action);

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
