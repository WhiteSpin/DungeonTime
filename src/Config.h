#include "System.h"

class Config {
	public:
	bool godMode = false;

	uint64_t levelWidth;
	uint64_t levelHeight;

	uint64_t minRoomWidth;
	uint64_t minRoomHeight;
	uint64_t maxRoomWidth;
	uint64_t maxRoomHeight;
};

extern std::unique_ptr<Config> config;
