#include "Human.h"

#define BACKGROUD_EMPTY ' '
#define BACKGROUD_FLOOR '.'
#define BACKGROUD_WALLX '-'
#define BACKGROUD_WALLY '|'
#define BACKGROUD_OPEN_DOOR 'x'
#define BACKGROUD_CLOSED_DOOR '+'
#define BACKGROUD_CORRIDOR '#'

class Room;
class Corridor;
class LevelElement;

class Level {
	public:
	uint64_t width, height;
	std::unique_ptr<uint8_t> background;
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Room>> rooms;
	std::vector<std::unique_ptr<Corridor>> corridors;
	std::vector<std::unique_ptr<LevelElement>> levelElements;
	Level();
	~Level();
	void doFrame();
	Entity* getPrimeEntityAt(uint64_t posX, uint64_t posY) const;
	LivingEntity* getLivingEntityAt(uint64_t posX, uint64_t posY, Entity* exceptFrom = NULL) const;
	ItemContainer* getItemContainerAt(uint64_t posX, uint64_t posY, Entity* exceptFrom = NULL) const;
	uint8_t getBackgroundAt(uint64_t posX, uint64_t posY) const;
	uint16_t* getWalkable();
	bool isWalkable(uint64_t posX, uint64_t posY);
	void setBackgroundAt(uint64_t posX, uint64_t posY, uint8_t type);
	void fillBackgroundRow(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type);
	void fillBackgroundColumn(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type);
	void fillBackgroundRect(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h, uint8_t type);
	void generateLine(uint64_t fromX, uint64_t fromY, uint64_t toX, uint64_t toY, uint8_t type);
	void generateEllipseRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateRectRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h, bool isPart = false);
	void generateXSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateYSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateXCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h, std::unique_ptr<LevelElement>(element1), std::unique_ptr<LevelElement>(element2));
	void generateYCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h, std::unique_ptr<LevelElement>(element1), std::unique_ptr<LevelElement>(element2));
	void generate();
	void generateRandom();
	void generateRooms();
	void generateConnections();
};

class LevelElement {
	public:
	uint64_t posX, posY;
	uint64_t width, height;
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


extern std::unique_ptr<Level> level;
extern LivingEntity* hero;
