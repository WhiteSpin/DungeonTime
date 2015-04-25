#include "Human.h"

#define BACKGROUD_EMPTY ' '
#define BACKGROUD_FLOOR '.'
#define BACKGROUD_WALLX '-'
#define BACKGROUD_WALLY '|'
#define BACKGROUD_OPEN_DOOR 'x'
#define BACKGROUD_CLOSED_DOOR '+'
#define BACKGROUD_CORRIDOR '#'

class Room;

class Level {
	public:
	uint64_t width, height;
	std::unique_ptr<uint8_t> background;
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Room>> rooms;
	Level();
	~Level();
	void doFrame();
	Entity* getPrimeEntityAt(uint64_t posX, uint64_t posY) const;
	LivingEntity* getLivingEntityAt(uint64_t posX, uint64_t posY, Entity* exceptFrom = NULL) const;
	ItemContainer* getItemContainerAt(uint64_t posX, uint64_t posY, Entity* exceptFrom = NULL) const;
	uint8_t getBackgroundAt(uint64_t posX, uint64_t posY) const;
	void setBackgroundAt(uint64_t posX, uint64_t posY, uint8_t type);
	void fillBackgroundRow(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type);
	void fillBackgroundColumn(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type);
	void fillBackgroundRect(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h, uint8_t type);
	void generateLine(uint64_t fromX, uint64_t fromY, uint64_t toX, uint64_t toY, uint8_t type);
	void generateEllipseRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateRectRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateXSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateYSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateXCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateYCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generate();
	void generateOther();
	void generateRandom();
};

class Room {
	public:
	uint64_t posX, posY;
	uint64_t width, height;
	Room(uint64_t posX, uint64_t posY, uint64_t width, uint64_t height);
};

extern std::unique_ptr<Level> level;
extern LivingEntity* hero;
