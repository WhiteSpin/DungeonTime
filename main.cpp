#include "Map.h"

int main(int argc, const char** argv) {
	struct termios old = {0};
	if(tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if(tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");

	map.generate();
	Entity hero;
	hero.posX = 17;
	hero.posY = 10;

	while(true) {
		uint8_t buffer[16];
		uint64_t readBytes = handleKeyboard(sizeof(buffer), buffer);
		if(readBytes == 3 && buffer[0] == 27 && buffer[1] == 91) {
			hero.handleControl(buffer[2]);
		}
		map.render();
		setCursorPosition(hero.posX, hero.posY);
		fflush(stdout);

		usleep(50000);
		//for(int i = 0; i < n; i ++)
		//	printf("%d %x %d\n", i, buffer[i], buffer[i]);
	}

	return 0;
}
