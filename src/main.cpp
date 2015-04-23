#include "Level.h"

int main(int argc, const char** argv) {
	System::init();
	level.reset(new Level());

	hero = new Human(level.get(), level->width/2+1, level->height/2+1);
	hero->name = "The Chosen One";
	hero->inventory->setItemInSlot(new Weapon(Weapon::WeaponType::Axe), 0);

	auto otherGuy = new Human(level.get(), level->width/2+2, level->height/2+1);
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
