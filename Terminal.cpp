#include "Terminal.h"

static const char* CSI = "\33[";

Terminal::Terminal() {
	struct termios old;
	if(tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if(tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
}

void Terminal::setCursorPosition(uint64_t posX, uint64_t posY) {
	printf("%s%llu;%lluf", CSI, posY+1, posX+1);
}

uint64_t Terminal::handleKeyboard(uint64_t bufferSize, uint8_t* buffer) {
	fd_set readset;
	struct timeval tv;
	FD_ZERO(&readset);
	FD_SET(STDIN_FILENO, &readset);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	select(STDIN_FILENO+1, &readset, NULL, NULL, &tv);
	if(FD_ISSET(STDIN_FILENO, &readset))
		return read(STDIN_FILENO, buffer, bufferSize);
	else
		return 0;
}

bool Terminal::isCSI(uint8_t* buffer) {
	return strncmp(CSI, (const char*)buffer, 2) == 0;
}

Terminal term;
