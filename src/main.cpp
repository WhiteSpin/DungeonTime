#include "Level.h"

int main(int argc, const char** argv) {
	System::init();
	int roomNum = 0;
	while(roomNum < 6) {
		level.reset(new Level());
		roomNum = level->rooms.size();
		//System::writeToLog("Rooms " + std::to_string(roomNum) + "\n");
	}
	/*
	for(auto&& i : level->corridors) {
		System::writeToLog("Pos X: " + std::to_string(i->connects.first->posX) + "Pos Y: " + std::to_string(i->connects.first->posX) + " connected with "
				+ "Pos X: " + std::to_string(i->connects.second->posX) + "Pos Y: " + std::to_string(i->connects.second->posX));
	}
	*/

	int roomIndex = rand() % level->rooms.size();
	auto heroRoom = level->rooms[roomIndex].get();
	hero = new Human(level.get(), heroRoom->posX+2, heroRoom->posY+2);
	hero->name = "The Chosen One";
	hero->inventory->setItemInSlot(new Weapon(Weapon::WeaponType::Axe), 0);
	/*
	for(int i = 0; i < level->width*level->height; ++i) {
		uint16_t* walkable = level->getWalkable();
		if(i%level->width == 0 && i!=0) {
			System::writeToLog("\n");
		}
		System::writeToLog(std::to_string(walkable[i]));
	}
	*/

	auto otherGuy = new Human(level.get(), level->width/2+3, level->height/2+2);
	otherGuy->name = "The Other One";
	otherGuy->friendly = false;

	/*
	auto yetanotherguy = new LivingEntity(level.get(), level->width/2+2, level->height/2+4);
	yetanotherguy->name = "The One With No Items";
	yetanotherguy->friendly = false;

	auto thirdGuy = new LivingEntity(level.get(), level->width/2+13, level->height/2+1);
	thirdGuy->name = "The Other Other One";
	thirdGuy->friendly = true;

	auto inv = new Inventory(6);
	inv->setItemInSlot(new Weapon(Weapon::WeaponType::Sword),0);
	inv->setItemInSlot(new Weapon(Weapon::WeaponType::Bow),1);
	auto container = new ItemContainer(level.get(), otherGuy->posX, otherGuy->posY, std::unique_ptr<Inventory>(inv));
	*/


	Controls::init();
	while(true) {
		System::doFrame();
		Controls::doFrame();
		for (auto& i : level->entities) {
			auto livingEntityAtPos = dynamic_cast<LivingEntity*>(i.get());
			if(livingEntityAtPos && livingEntityAtPos->name != "The Chosen One") {
				uint16_t field [level->width*level->height];
				level->createField(hero->posX, hero->posY, field);
				livingEntityAtPos->goTowards(field);
			}
			auto movingItemContainerAtPos = dynamic_cast<MovingItemContainer*>(i.get());
			if(movingItemContainerAtPos) {
				System::writeToLog(std::to_string(reinterpret_cast<uint64_t>(movingItemContainerAtPos))+" ");
				auto newPos = *movingItemContainerAtPos->path.begin();
				livingEntityAtPos = level->getLivingEntityAt(newPos.first, newPos.second);
				movingItemContainerAtPos->path.erase(movingItemContainerAtPos->path.begin());
				if(!movingItemContainerAtPos->path.empty() && level->isWalkable(newPos.first, newPos.second)) {
					if(livingEntityAtPos) {
						Message::push("Hit LivingEntity"); // TODO testing
					} else {
						movingItemContainerAtPos->posX = newPos.first;
						movingItemContainerAtPos->posY = newPos.second;
						System::writeToLog("x: " + std::to_string(newPos.first) + "y: " + std::to_string(newPos.second) + "\n"); // TODO testing
					}
				} else {
					movingItemContainerAtPos->land();
				}
			}
		}
		level->doFrame();
		Message::doFrame();
		System::setCursorPosition(hero->posX, hero->posY);
	}
	return 0;
}
