#include "Controls.h"

uint8_t asciiKeyBinding[128];
uint8_t ansiKeyBinding[128];
Entity* hero;

void Controls::init() {
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

	hero = new Entity(level.get(), 17, 10);
}

void Controls::doFrame() {
	Entity::Action action;
	uint8_t buffer[16];
	uint64_t readBytes = System::handleKeyboard(sizeof(buffer), buffer);
	if(readBytes == 3 && System::isCSI(buffer))
		action = (Entity::Action)ansiKeyBinding[buffer[2]];
	else if(readBytes == 1)
		action = (Entity::Action)asciiKeyBinding[buffer[0]];
	if(readBytes)
		hero->handleAction(action);
}
