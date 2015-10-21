#include "Level.h"

Controls::Mode mode = Controls::Move;
uint8_t asciiKeyBinding[128];
uint8_t ansiKeyBinding[128];
std::string inputText;


class Selection;
std::unique_ptr<Selection> selection;

class Selection {
	public:
	Item* item;
	Selection* activeSelection;
	std::vector<std::unique_ptr<Selection>> contains;

	Selection(Item* _item) :item(_item), activeSelection(NULL) {

	}

	static void printInner(Item* item, uint64_t depth, uint64_t& line) {
		if(!item)
			return;

		std::stringstream ss;
		ss << item->getDescription() << " <";
		for(uint64_t i = 0; i < depth; i ++)
			ss << "=";
		System::renderRightAlignedText(line ++, ss.str().c_str());

		Inventory* inventory = dynamic_cast<Inventory*>(item);
		if(!inventory)
			return;

		depth ++;
		for(uint64_t i = 0; i < inventory->getSlotCount(); i ++)
			printInner(inventory->getItemInSlot(i), depth, line);
	}

	static void removeActive() {
		inputText = "";
		Selection* active = selection.get();
		if(!active->activeSelection)
			mode = Controls::Move;
		else
			while(active && active->activeSelection) {
				if(!active->activeSelection->activeSelection) {
					delete active->activeSelection;
					active->activeSelection = NULL;
					return;
				}
				active = active->activeSelection;
			}
	}

	static void autoComplete() {
		Selection* active = selection.get();
		while(active) {
			if(!active->activeSelection) {
				Inventory* inventory = dynamic_cast<Inventory*>(active->item);
				if(inventory) {
					std::string completion = inputText;
					std::vector<std::pair<Item*, std::string>> similar;
					for(uint64_t i = 0; i < inventory->getSlotCount(); ++i) {
						Item* item = inventory->getItemInSlot(i);
						if(!item) continue;
						std::string description = item->getDescription();
						if(description.length() >= completion.length() &&
						   description.compare(0, completion.length(), completion) == 0)
							similar.push_back(std::pair<Item*, std::string>(item, description));
					}
					std::sort(similar.begin(), similar.end(), [](std::pair<Item*, std::string> a, std::pair<Item*, std::string> b) {
						return (bool)(a.second.length() < b.second.length());
					});
					while(similar.size()) {
						if(similar.size() == 1 || similar[0].second.length() == completion.length()) {
							active->activeSelection = new class Selection(similar[0].first);
							inputText = "";
							return;
						}else{
							completion += similar[0].second[completion.size()];
							for(uint64_t i = 0; i < similar.size(); ++i)
								if(similar[i].second.compare(0, completion.length(), completion) != 0) {
									similar.erase(similar.begin()+i);
									--i;
								}
						}
					}
					inputText = completion;
				}
				return;
			}
			active = active->activeSelection;
		}
	}

	static void print() {
		uint64_t line = 3;
		Selection* active = selection.get();
		printInner(active->item, 0, line);

		std::stringstream ss;
		while(active) {
			Item* item = active->item;
			ss << item->getDescription() << "/";
			active = active->activeSelection;
		}
		ss << inputText;

		System::renderRightAlignedText(0, "Command:");
		System::renderRightAlignedText(1, ss.str().c_str());
	}
};



void Controls::init() {
	memset(asciiKeyBinding, 0, sizeof(asciiKeyBinding));
	asciiKeyBinding['k'] = Action::Up;
	asciiKeyBinding['j'] = Action::Down;
	asciiKeyBinding['l'] = Action::Right;
	asciiKeyBinding['h'] = Action::Left;
	asciiKeyBinding['t'] = Action::Throw;
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
		case Mode::Move:
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
				case Action::Throw:
					hero->handleAction(action);
				break;
				case Action::Cancel:
					System::terminate();
				break;
				default:;
			}
		break;
		case Mode::Command:
			switch(action) {
				case Action::Delete:
					if(inputText.size() > 0)
						inputText.erase(inputText.end()-1);
					else
						Selection::removeActive();
				break;
				case Action::Cancel:
					Selection::removeActive();
				break;
				case Action::Confirm:
					Selection::autoComplete();
				break;
				default:
					inputText += input[0];
				break;
			}
		break;
	}
}

void Controls::doFrame() {
	Action action = NotAssigned;

	{
		uint8_t buffer[32], *pos = buffer;
		uint64_t readBytes = sizeof(buffer);
		bool isCSI = System::pollKeyboard(readBytes, buffer);

		while(pos < buffer+readBytes) {
			if(isCSI) {
				action = (Action)ansiKeyBinding[pos[2]];
				handleInput(pos, action);
				pos += 3;
			}else{
				if(pos[0] < 128) {
					action = (Action)asciiKeyBinding[pos[0]];
					handleInput(pos, action);
				}
				pos += 1;
			}
		}
	}

	char buffer[System::screenSize.ws_col];
	switch(mode) {
		case Mode::Move:
			System::renderRightAlignedText(0, "Move");
		break;
		case Mode::Command:
			Selection::print();
		break;
	}
}
