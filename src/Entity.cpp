#include "Level.h"
#include "Greetings.h"


Entity::Entity(Level* _level, uint64_t _posX, uint64_t _posY, std::unique_ptr<Inventory> _inventory)
	:level(_level), posX(_posX), posY(_posY), inventory(std::move(_inventory)) {
	level->entities.push_back(std::unique_ptr<Entity>(this));
}

bool Entity::destroy() {
	for(uint64_t i = 0; i < level->entities.size(); ++i)
		if(level->entities[i].get() == this) {
			level->entities.erase(level->entities.begin()+i);
			return true;
		}
	return false;
}

void Entity::doFrame() {
	printf("@");
}

bool Entity::tryToEnter(uint64_t posX, uint64_t posY) {
	LivingEntity* livingEntityAtPos = level->getLivingEntityAt(posX, posY);
	if(livingEntityAtPos) {
		if(livingEntityAtPos->friendly) {
			if(livingEntityAtPos->willTalk) {
				Message::push(get_random_greeting());
				livingEntityAtPos->willTalk = false;
			}
			return false;
		}else{
			Message::push(std::string("You hit ") + (livingEntityAtPos->name));
			Weapon* weapon = dynamic_cast<Weapon*>(inventory->getItemInSlot(0));
			if(weapon) {
				Message::push(livingEntityAtPos->name + " -" + std::to_string(weapon->getDamage()));
				if(livingEntityAtPos->hurt(weapon->getDamage())) {
					Message::push("You kill " + livingEntityAtPos->name);
				}
			}
			return false;
		}
	}else{
		ItemContainer* ItemContainerAtPos = level->getItemContainerAt(posX, posY);
		if(ItemContainerAtPos && ItemContainerAtPos->inventory->getFilledSlotsCount() > 0) {
			ItemContainerAtPos->inventory->printFilledSlots();
		}
	}

	switch(level->getBackgroundAt(posX, posY)) {
		case BACKGROUD_CLOSED_DOOR:
			return false;
		case BACKGROUD_FLOOR:
		case BACKGROUD_OPEN_DOOR:
			return true;
		default:
			return false;
	}
	return false;
}

bool Entity::handleAction(Controls::Action input) {
	switch(input) {
		case Controls::Up:
			if(posY > 0 && tryToEnter(posX, posY-1))
				posY --;
			return true;
		case Controls::Down:
			if(posY < level->height-1 && tryToEnter(posX, posY+1))
				posY ++;
			return true;
		case Controls::Left:
			if(posX > 0 && tryToEnter(posX-1, posY))
				posX --;
			return true;
		case Controls::Right:
			if(posX < level->width-1 && tryToEnter(posX+1, posY))
				posX ++;
			return true;
		case Controls::Throw: {
			auto inv = new Inventory(1);
			inv->setItemInSlot(new Weapon(Weapon::WeaponType::Sword), 0);
			auto livingEntityAtPos = dynamic_cast<LivingEntity*>(this);
			livingEntityAtPos->throwItem(std::unique_ptr<Inventory>(inv), posX+10, posY);
			} return true;
		case Controls::Door: {
			auto neighbors = level->getNeighborFields(hero->posX, hero->posY);
			for(auto& i : neighbors) {
				if(level->getBackgroundAt(i.first, i.second) == BACKGROUD_CLOSED_DOOR) {
					level->setBackgroundAt(i.first, i.second, BACKGROUD_OPEN_DOOR);
					Message::push(std::string("The door opens"));
				}
				else {
					if(level->getBackgroundAt(i.first, i.second) == BACKGROUD_OPEN_DOOR) {
						level->setBackgroundAt(i.first, i.second, BACKGROUD_CLOSED_DOOR);
						Message::push(std::string("The door closes"));
					}
				}
			}
			} return true;
		default:
			Message::push(std::string("Unknown command!"));
			return false;
	}
}

LivingEntity::LivingEntity(Level* _level, uint64_t _posX, uint64_t _posY)
	:Entity(_level, _posX, _posY), maxHealth(12.0) {
	health = maxHealth;
	willTalk = true;
}

bool LivingEntity::hurt(int damage) {
	health -= damage;
	if(health <= 0) {
		if(inventory && inventory->getFilledSlotsCount() != 0)
			auto container = new ItemContainer(level, posX, posY, std::move(inventory));
		return destroy();
	}
	return false;
}

int LivingEntity::heal(int value) {
	if(health+value > maxHealth) {
		value = maxHealth-health;
		health = maxHealth;
		return value;
	}else{
		health += value;
		return 0;
	}
}


bool LivingEntity::goTowards(uint16_t *field) {
	uint16_t min = field[posY*level->width+posX];
	std::pair<uint64_t, uint64_t> p;

	if(field[posY*level->width+posX+1] < min) {
		min = field[posY*level->width+posX+1];
		p = {posX+1, posY};
	}
	if(field[(posY+1)*level->width+posX] < min) {
		min = field[(posY+1)*level->width+posX];
		p = {posX, posY+1};
	}
	if(field[posY*level->width+posX-1] < min) {
		min = field[posY*level->width+posX-1];
		p = {posX-1, posY};
	}
	if(field[(posY-1)*level->width+posX]< min) {
		min = field[(posY-1)*level->width+posX];
		p = {posX, posY-1};
	}

	if(min < field[posY*level->width+posX] && !dynamic_cast<LivingEntity*>(level->getLivingEntityAt(p.first, p.second))) {
		this->posX = p.first;
		this->posY = p.second;
		return true;
	}
	return false;
}


ItemContainer::ItemContainer(Level* _level, uint64_t _posX, uint64_t _posY, std::unique_ptr<Inventory> _inventory)
	:Entity(_level, _posX, _posY, std::move(_inventory)) {

	Entity* itemContainerAtPos = level->getItemContainerAt(posX, posY, this);
	if(itemContainerAtPos) {
		for(uint64_t i = 0; i < itemContainerAtPos->inventory->getSlotCount(); ++i) {
			auto itemInSlot = std::move(itemContainerAtPos->inventory->items[i]);
			if(itemInSlot)
				inventory->items.push_back(std::move(itemInSlot));
		}
		itemContainerAtPos->destroy();
	}
}

void ItemContainer::doFrame() {
	for(size_t i = 0; i < inventory->items.size(); ++i) {
		auto item = inventory->getItemInSlot(i);
		if(!item)
			continue;
		printf("%c", item->getApperance());
		return;
	}
	printf("%c",level->getBackgroundAt(posX,posY));
}

MovingItemContainer::MovingItemContainer(Level* _level, uint64_t _posX, uint64_t _posY, std::unique_ptr<Inventory> _inventory)
	:ItemContainer(_level, _posX, _posY, std::move(_inventory)) {
}

void MovingItemContainer::land() {
	std::unique_ptr<Inventory> inv = std::move(inventory);
	uint64_t _posX = posX;
	uint64_t _posY = posY;
	destroy();
	auto itemContainer = new ItemContainer(level, _posX, _posY, std::move(inv));
}

void LivingEntity::throwItem(std::unique_ptr<Inventory> inv, uint64_t targetX, uint64_t targetY) {
	auto path = level->calculateLine(posX, posY, targetX, targetY);
	auto container = new MovingItemContainer(level, path.begin()->first, path.begin()->second, std::move(inv));
	path.erase(path.begin());
	container->path = path;
}
