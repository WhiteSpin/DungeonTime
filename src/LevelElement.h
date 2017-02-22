#include "Human.h"
class Door;

class LevelElement {
	public:
	uint64_t posX, posY;
	uint64_t width, height;
	bool visible;
	std::vector<std::unique_ptr<Door>> doors;

	LevelElement(uint64_t posX, uint64_t posY, uint64_t width, uint64_t height);
	static bool checkCollision(LevelElement* a, LevelElement* b);
};

class Room : public LevelElement{
	public:
	enum RoomType {
		EllipseRoom,
		RectRoom,
		XSplitRoom,
		YSplitRoom,	
	} type;
	Room(uint64_t posX, uint64_t posY, uint64_t width, uint64_t height, RoomType type);
	~Room();
};

class Corridor : public LevelElement {
	public:
	enum CorridorType {
		YCorridor,
		XCorridor
	} type;
	std::pair<std::unique_ptr<LevelElement>,std::unique_ptr<LevelElement>> connects;
	Corridor(uint64_t posX, uint64_t posY, uint64_t width, uint64_t height, CorridorType type);
};

class Door {
	public:
	uint64_t relPosX, relPosY;
	Room* origin;
	Room* destination;
	Door(uint64_t relPosX, uint64_t relPosY, Room* origin);
};

