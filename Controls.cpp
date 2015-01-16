#include "Level.h"

uint8_t asciiKeyBinding[128];
uint8_t ansiKeyBinding[128];

void Controls::init() {
	memset(asciiKeyBinding, 0, sizeof(asciiKeyBinding));
	asciiKeyBinding['k'] = Controls::Up;
	asciiKeyBinding['j'] = Controls::Down;
	asciiKeyBinding['l'] = Controls::Right;
	asciiKeyBinding['h'] = Controls::Left;

	memset(ansiKeyBinding, 0, sizeof(ansiKeyBinding));
	ansiKeyBinding['A'] = Controls::Up;
	ansiKeyBinding['B'] = Controls::Down;
	ansiKeyBinding['C'] = Controls::Right;
	ansiKeyBinding['D'] = Controls::Left;
}

void Controls::doFrame() {
	Controls::Action action;

	{
		uint8_t buffer[16];
		uint64_t readBytes = System::handleKeyboard(sizeof(buffer), buffer);

		if(readBytes == 3 && System::isCSI(buffer))
			action = (Controls::Action)ansiKeyBinding[buffer[2]];
		else if(readBytes == 1)
			action = (Controls::Action)asciiKeyBinding[buffer[0]];

		if(readBytes)
			hero->handleAction(action);
	}

	{
		char buffer[System::screenSize.ws_col];
		System::renderRightAlignedText(0, "Inventory");
		for(uint64_t i = 0; i < hero->inventory.size(); i ++) {
			if(hero->inventory[i].get())
				sprintf(buffer, "%llu (%s) : %s", i+1, hero->getInventorySlotName(i), hero->inventory[i]->getDescription().c_str());
			else
				sprintf(buffer, "%llu (%s) : -", i+1, hero->getInventorySlotName(i));
			System::renderRightAlignedText(i+1, buffer);
		}
	}

	/*buffer[readBytes] = 0;
	printf("%s\n", buffer);
	for(int i = 0; i < readBytes; i ++)
		printf("%d %x %d\n", i, buffer[i], buffer[i]);*/
}
