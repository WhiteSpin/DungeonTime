#include "Map.h"

int main(int argc, const char** argv) {
	map.generate();
	Entity hero;
	hero.posX = 17;
	hero.posY = 10;

	while(true) {
		uint8_t buffer[16];
		uint64_t readBytes = term.handleKeyboard(sizeof(buffer), buffer);
		if(readBytes == 3 && term.isCSI(buffer)) {
			hero.handleControl(buffer[2]);
		}
		map.render();
		term.setCursorPosition(hero.posX, hero.posY);
		fflush(stdout);

		usleep(50000);
		//for(int i = 0; i < n; i ++)
		//	printf("%d %x %d\n", i, buffer[i], buffer[i]);
	}

	return 0;
}
