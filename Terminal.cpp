#include "Map.h"

System::TextAttribute System::textAttribute;
System::Color System::foreground, System::background;
double System::frameDuration, lastTime;
struct termios original;
struct winsize screenSize;
static const char* CSI = "\33[";

void System::init() {
	tcgetattr(0, &original);
	signal(SIGINT, (void(*)(int))System::terminate);
	ioctl(0, TIOCGWINSZ, &screenSize);
	System::eraseStartingAtLine(0);

	struct termios aux;
	tcgetattr(0, &aux);
	aux.c_lflag &= ~ICANON;
	aux.c_lflag &= ~ECHO;
	aux.c_cc[VMIN] = 1;
	aux.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &aux);

	textAttribute = ResetAll;
	foreground = White;
	background = Black;
	lastTime = getTime();
}

void System::terminate() {
	eraseStartingAtLine(0);
	textAttribute = ResetAll;
	foreground = Default;
	background = Default;
	setTextStyle();
	tcsetattr(0, TCSANOW, &original);
	exit(0);
}

void System::eraseStartingAtLine(uint64_t atLine) {
	setCursorPosition(0, atLine);
	for(uint64_t y = atLine; y < screenSize.ws_row-1; ++y) {
		for(uint64_t x = 0; x < screenSize.ws_col; ++x)
			printf(" ");
		printf("\n");
	}
	setCursorPosition(0, atLine);
}

void System::setCursorPosition(uint64_t posX, uint64_t posY) {
	printf("%s%llu;%lluf", CSI, posY+1ULL, posX+1ULL);
}

void System::setTextStyle() {
	printf("%s%u;%llu;%llum", CSI, textAttribute, foreground+30ULL, background+40ULL);
}

uint64_t System::handleKeyboard(uint64_t bufferSize, uint8_t* buffer) {
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

bool System::isCSI(uint8_t* buffer) {
	return strncmp(CSI, (const char*)buffer, 2) == 0;
}

double System::getTime() {
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec + now.tv_usec/1000000.0;
}

void System::doFrame() {
	fflush(stdout);
	usleep(50000);
	ioctl(0, TIOCGWINSZ, &screenSize);
	setCursorPosition(0, 0);
	setTextStyle();

	double now = getTime();
	frameDuration = now-lastTime;
	lastTime = now;
}
