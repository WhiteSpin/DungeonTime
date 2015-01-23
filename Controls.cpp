#include "Level.h"

Controls::Mode mode = Controls::Move;
uint8_t asciiKeyBinding[128];
uint8_t ansiKeyBinding[128];
std::vector<uint64_t> itemSelection;
std::string commandLine;

void Controls::init() {
	memset(asciiKeyBinding, 0, sizeof(asciiKeyBinding));
	asciiKeyBinding['k'] = Action::Up;
	asciiKeyBinding['j'] = Action::Down;
	asciiKeyBinding['l'] = Action::Right;
	asciiKeyBinding['h'] = Action::Left;
	asciiKeyBinding[127] = Action::Delete;
	asciiKeyBinding[27] = Action::Cancel;
	asciiKeyBinding[10] = Action::Confirm;
	asciiKeyBinding['u'] = Action::Primary;
	asciiKeyBinding['i'] = Action::Secondary;
	for(uint64_t i = 0; i < 9; i ++)
		asciiKeyBinding['1'+i] = Action::SelectSlot+i;

	memset(ansiKeyBinding, 0, sizeof(ansiKeyBinding));
	ansiKeyBinding['A'] = Action::Up;
	ansiKeyBinding['B'] = Action::Down;
	ansiKeyBinding['C'] = Action::Right;
	ansiKeyBinding['D'] = Action::Left;
}

bool Controls::tryToSelectItemSlot(Action action) {
	if(action < Action::SelectSlot ||
	   action >= Action::SelectSlot+hero->inventory->items.size())
		return false;

	uint64_t index = action-Action::SelectSlot;
	auto iterator = std::find(itemSelection.begin(), itemSelection.end(), index);
	if(iterator != itemSelection.end()) {
		itemSelection.erase(iterator);
		if(itemSelection.size() == 0)
			mode = Mode::Move;
	}else{
		itemSelection.push_back(index);
		mode = Mode::ItemSelection;
	}
	return true;
}

void Controls::handleInput(uint8_t* input, Action action) {
	switch(mode) {
		case Mode::Move: {
			if(!tryToSelectItemSlot(action))
				switch(action) {
					case Action::Up:
					case Action::Down:
					case Action::Right:
					case Action::Left:
						hero->handleAction(action);
					break;
					case Action::Confirm:
						mode = Mode::Command;
					break;
					case Action::Cancel:
						System::terminate();
					break;
					default:;
				}
		} break;
		case Mode::Command: {
			switch(action) {
				case Action::Confirm:
					commandLine = "";
				break;
				case Action::Delete:
					if(commandLine.size() > 0)
						commandLine.erase(commandLine.end()-1);
				break;
				case Action::Cancel:
					commandLine = "";
					mode = Mode::Move;
				break;
				default:
					commandLine += input[0];
				break;
			}
		} break;
		case Mode::ItemSelection: {
			if(!tryToSelectItemSlot(action) && action == Action::Cancel) {
				mode = Mode::Move;
				itemSelection.clear();
				break;
			}
		} break;
	}
}

void Controls::doFrame() {
	Action action = NotAssigned;

	{
		uint8_t buffer[32], *pos = buffer;
		uint64_t readBytes = System::handleKeyboard(sizeof(buffer), buffer);

		/*pos[readBytes] = 0;
		printf("NEXT\n");
		for(int i = 0; i < readBytes; i ++)
			printf("%d %x %d\n", i, pos[i], pos[i]);*/

		while(readBytes) {
			if(readBytes >= 3 && System::isCSI(pos)) {
				action = (Action)ansiKeyBinding[pos[2]];
				handleInput(pos, action);
				readBytes -= 3; pos += 3;
			}else{
				if(pos[0] < 128) {
					action = (Action)asciiKeyBinding[pos[0]];
					handleInput(pos, action);
				}
				readBytes -= 1; pos += 1;
			}
		}
	}

	char buffer[System::screenSize.ws_col];
	switch(mode) {
		case Mode::Move: {
			System::renderRightAlignedText(0, "Move");
		} break;
		case Mode::Command: {
			System::renderRightAlignedText(0, "Command:");
			System::renderRightAlignedText(1, commandLine.c_str());
		} break;
		case Mode::ItemSelection: {
			System::renderRightAlignedText(0, "Selected Items:");
			float massSum = 0.0;
			for(uint64_t i = 0; i < itemSelection.size(); i ++) {
				uint64_t slot = itemSelection[i];
				strcpy(buffer, hero->inventory->getSlotDescription(slot).c_str());
				System::renderRightAlignedText(i+1, buffer);
				Item* item = hero->inventory->getItemInSlot(slot);
				if(item)
					massSum += item->getMass();
			}
			uint64_t line = itemSelection.size()+2;
			sprintf(buffer, "%.f Kg", massSum);
			System::renderRightAlignedText(line, buffer);
			System::renderRightAlignedText(line+1, "Drop");
			System::renderRightAlignedText(line+2, "Apply");
			System::renderRightAlignedText(line+3, "Swap");
		} break;
	}
}
