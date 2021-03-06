#include "LevelElement.h"

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
	bool godMode = false;
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
	bool isInBounds(uint64_t posX, uint64_t posY);
	std::vector<std::pair<uint64_t, uint64_t>> getNeighborFields(uint64_t posX, uint64_t posY);
	bool isVisible(uint64_t posX, uint64_t posY, uint16_t* field);
	bool isWalkable(uint64_t posX, uint64_t posY);
	void createDistanceField(uint64_t posX, uint64_t posY, uint16_t* field);
	void createVisibilityField(uint64_t posX, uint64_t posY, uint16_t* field);
	void setBackgroundAt(uint64_t posX, uint64_t posY, uint8_t type);
	void fillBackgroundRow(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type);
	void fillBackgroundColumn(uint64_t posX, uint64_t posY, uint64_t length, uint8_t type);
	void fillBackgroundRect(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h, uint8_t type);
	std::vector<std::pair<uint64_t, uint64_t>> calculateLine(uint64_t fromX, uint64_t fromY, uint64_t toX, uint64_t toY);
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


extern std::unique_ptr<Level> level;
extern LivingEntity* hero;
