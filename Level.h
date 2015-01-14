#include "Entity.h"

#define BACKGROUD_EMPTY ' '
#define BACKGROUD_FLOOR '.'
#define BACKGROUD_CORRIDOR '#'

class Level {
	public:
	uint64_t width, height;
	std::unique_ptr<uint8_t> background;
	std::vector<std::unique_ptr<Entity>> entities;
	Level();
	~Level();
	void doFrame();
	Entity* getEntityAt(uint64_t posX, uint64_t posY);
	uint8_t getBackgroundAt(uint64_t posX, uint64_t posY);
	bool replaceBackgroundCell(uint8_t type, uint64_t posX, uint64_t posY);
	void fillBackgroundRow(uint8_t type, uint64_t posX, uint64_t posY, uint64_t length);
	void fillBackgroundColumn(uint8_t type, uint64_t posX, uint64_t posY, uint64_t length);
	void fillBackgroundRect(uint8_t type, uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateXSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateYSplitRoom(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateXCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generateYCorridor(uint64_t posX, uint64_t posY, uint64_t w, uint64_t h);
	void generate();
};

extern std::unique_ptr<Level> level;
extern Entity* hero;
