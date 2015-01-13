#include "Terminal.h"

static const char* CSI = "\33[";

void setCursorPosition(uint64_t posX, uint64_t posY) {
	printf("%s%llu;%lluf", CSI, posY+1, posX+1);
}

uint64_t handleKeyboard(uint64_t bufferSize, uint8_t* buffer) {
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
