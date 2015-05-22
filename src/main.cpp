#include "Level.h"

int main(int argc, const char** argv) {
	System::init();
	/*
	int roomNum = 0;
	while(roomNum < 6) { 
		level.reset(new Level());
		roomNum = level->rooms.size();
	}
	*/
	level.reset(new Level());
	for(auto&& i : level->corridors) {
		//System::writeToLog("Pos X: " + std::to_string(i->connects.first->posX) + "Pos Y: " + std::to_string(i->connects.first->posX) + " connected with " 
		//		+ "Pos X: " + std::to_string(i->connects.second->posX) + "Pos Y: " + std::to_string(i->connects.second->posX));
	}

	/*
	int roomIndex = rand() % level->rooms.size();
	auto heroRoom = level->rooms[roomIndex].get();
	*/
	hero = new Human(level.get(), 40, 7);
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
			if(livingEntityAtPos && livingEntityAtPos->name != "The Chosen One")
				livingEntityAtPos->goTowards(hero->posX, hero->posY);
		}
		level->doFrame();
		Message::doFrame();
		System::setCursorPosition(hero->posX, hero->posY);
	}

	return 0;
}
