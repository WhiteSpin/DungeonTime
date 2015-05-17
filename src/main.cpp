#include "Level.h"

int main(int argc, const char** argv) {
	System::init();
	int roomNum = 0;
	while(roomNum < 5) { 
		level.reset(new Level());
		roomNum = level->rooms.size();
	}

	int roomIndex = rand() % level->rooms.size();
	auto heroRoom = level->rooms[roomIndex].get();
	hero = new Human(level.get(), heroRoom->posX+2, heroRoom->posY+2);
	hero->name = "The Chosen One";
	hero->inventory->setItemInSlot(new Weapon(Weapon::WeaponType::Axe), 0);

	/*
	auto otherGuy = new Human(level.get(), level->width/2+3, level->height/2+2);
	otherGuy->name = "The Other One";
	otherGuy->friendly = false;

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
		level->doFrame();
		Message::doFrame();
		System::setCursorPosition(hero->posX, hero->posY);
	}

	return 0;
}
