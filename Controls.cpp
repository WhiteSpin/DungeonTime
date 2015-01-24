#include "Level.h"

Controls::Mode mode = Controls::Move;
uint8_t asciiKeyBinding[128];
uint8_t ansiKeyBinding[128];
std::string commandLine;


class Selection;
std::unique_ptr<Selection> selection;
uint64_t selectionIndex = 0;

class Selection {
	public:
	Item* item;
	Selection* activeSelection;
	std::vector<std::unique_ptr<Selection>> contains;

	Selection(Item* _item) :item(_item), activeSelection(NULL) {

	}

	static void printInner(std::stringstream& ss, Item* item, uint64_t depth) {
		ss << item->getDescription() << " |";
		for(uint64_t i = 0; i < depth; i ++)
			ss << "=";

		Inventory* inventory = dynamic_cast<Inventory*>(item);
		if(!inventory)
			return;
		depth ++;
		for(uint64_t i = 0; i < inventory->getSlotCount(); i ++)
			printInner(ss, inventory->getItemInSlot(i), depth);
	}

	static void removeActive() {
		selectionIndex = 0;
		Selection* active = selection.get();
		while(active && active->activeSelection) {
			if(!active->activeSelection->activeSelection) {
				delete active->activeSelection;
				active->activeSelection = NULL;
				return;
			}
			active = active->activeSelection;
		}
	}

	static void addActive() {
		Selection* active = selection.get();
		while(active) {
			if(!active->activeSelection) {
				Inventory* inventory = dynamic_cast<Inventory*>(active->item);
				if(!inventory || inventory->getSlotCount() <= selectionIndex)
					return;
				active->activeSelection = new class Selection(inventory->getItemInSlot(selectionIndex));
			}
			active = active->activeSelection;
		}
		selectionIndex = 0;
	}

	static bool handleInput(Controls::Action action) {
		switch(action) {
			case Controls::Delete:
				selectionIndex = selectionIndex/10;
			return true;
			case Controls::Cancel:
				removeActive();
			return true;
			case Controls::Confirm:
				addActive();
			return true;
			default:
				if(action < Controls::Select0 || action > Controls::Select9)
					return false;

				selectionIndex = selectionIndex*10+action-Controls::Select0;
				//if(selectionIndex == 0)
				//	addActive();
			return true;
		}
		return false;
	}

	static std::string getActivePath() {
		std::stringstream ss;
		Selection* active = selection.get();
		while(active) {
			Item* item = active->item;
			ss << item->getDescription() << "/";
			active = active->activeSelection;
		}
		if(selectionIndex)
			ss << selectionIndex;
		return ss.str();
	}
};



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
	for(uint64_t i = 0; i <= 9; i ++)
		asciiKeyBinding['0'+i] = Action::Select0+i;

	memset(ansiKeyBinding, 0, sizeof(ansiKeyBinding));
	ansiKeyBinding['A'] = Action::Up;
	ansiKeyBinding['B'] = Action::Down;
	ansiKeyBinding['C'] = Action::Right;
	ansiKeyBinding['D'] = Action::Left;

	selection.reset(new class Selection(hero->inventory.get()));
}

void Controls::handleInput(uint8_t* input, Action action) {
	switch(mode) {
		case Mode::Move: {
			if(!Selection::handleInput(action))
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
		case Mode::Selection: {
			if(!Selection::handleInput(action) && action == Action::Cancel) {
				mode = Mode::Move;
				selection.reset();
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
		case Mode::Selection: {
			std::string str = Selection::getActivePath();
			System::renderRightAlignedText(0, "Selection:");
			System::renderRightAlignedText(1, str.c_str());
		} break;
	}
}
