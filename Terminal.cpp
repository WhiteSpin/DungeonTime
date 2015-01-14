#include "Terminal.h"

struct termios original;
static const char* CSI = "\33[";

void Terminal::init() {
	tcgetattr(0, &original);
	signal(SIGINT, (void(*)(int))Terminal::terminate);

	struct termios aux;
	tcgetattr(0, &aux);
	aux.c_lflag &= ~ICANON;
	aux.c_lflag &= ~ECHO;
	aux.c_cc[VMIN] = 1;
	aux.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &aux);
}

void Terminal::terminate() {
	Terminal::clearScreen();
	tcsetattr(0, TCSANOW, &original);
	exit(0);
}

void Terminal::clearScreen() {
	Terminal::setCursorPosition(0, 0);
	printf("%s0J", CSI);
}

void Terminal::setCursorPosition(uint64_t posX, uint64_t posY) {
	printf("%s%llu;%lluf", CSI, posY+1ULL, posX+1ULL);
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
