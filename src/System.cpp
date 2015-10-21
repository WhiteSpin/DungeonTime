#include "Level.h"

std::ofstream logFile;
System::TextAttribute System::textAttribute;
System::Color System::foreground, System::background;
double System::frameDuration, lastTime;
struct termios System::termiosOld, System::termiosNew;
struct winsize System::screenSize;
static const char* CSI = "\33["; // "\e["

void System::init() {
	tcgetattr(STDIN_FILENO, &termiosOld);
	tcgetattr(STDIN_FILENO, &termiosNew);
	termiosNew.c_lflag &= ~ICANON;
	termiosNew.c_lflag &= ~ECHO;
	termiosNew.c_cc[VMIN] = 1;
	termiosNew.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &termiosNew);
	ioctl(STDIN_FILENO, TIOCGWINSZ, &screenSize);
	signal(SIGINT, (void(*)(int))System::terminate);

	logFile.open("logfile.log", std::ofstream::out);
	lastTime = getTime();
}

void System::terminate() {
	textAttribute = ResetAll;
	foreground = Default;
	background = Default;
	setTextStyle();
	tcsetattr(0, TCSANOW, &termiosOld);
	eraseScreen();
	setCursorPosition(0, 0);
	exit(0);
}

void System::eraseScreen() {
	// printf("%s2J", CSI);
	for(uint64_t y = 0; y < screenSize.ws_row; ++y) {
		setCursorPosition(0, y);
		printf("%s2K", CSI);
	}
}

void System::setCursorPosition(uint64_t posX, uint64_t posY) {
	printf("%s%llu;%lluf", CSI, posY+1ULL, posX+1ULL);
}

void System::setTextStyle() {
	printf("%s%u;%llu;%llum", CSI, textAttribute, foreground+30ULL, background+40ULL);
}

void System::renderRightAlignedText(uint64_t row, const char* text) {
	if(row >= screenSize.ws_row)
		return;

	int64_t len = strlen(text), col = screenSize.ws_col-len;
	if(col <= level->width)
		col = level->width+1;
	len = screenSize.ws_col-col;

	setCursorPosition(col, row);
	if(len > 0)
		fwrite(text, 1, len, stdout);
}

void System::writeToLog(const std::string &str) {
	writeToLog(str.c_str());
}

void System::writeToLog(const char *str) {
	logFile << str;
	logFile.flush();
}

bool System::pollKeyboard(uint64_t& bufferSize, uint8_t* buffer) {
	fd_set readset;
	struct timeval tv;
	FD_ZERO(&readset);
	FD_SET(STDIN_FILENO, &readset);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	select(STDIN_FILENO+1, &readset, NULL, NULL, &tv);

	if(FD_ISSET(STDIN_FILENO, &readset)) {
		bufferSize = read(STDIN_FILENO, buffer, bufferSize);
		return (bufferSize > 2 && strncmp(CSI, reinterpret_cast<const char*>(buffer), 2) == 0);
	}else{
		bufferSize = 0;
		return false;
	}
}

double System::getTime() {
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec + now.tv_usec/1000000.0;
}

void System::doFrame() {
	fflush(stdout);
	const size_t fps = 8;
	usleep(1000000/fps);

	struct winsize newScreenSize;
	ioctl(0, TIOCGWINSZ, &newScreenSize);
	if(newScreenSize.ws_col != screenSize.ws_col ||
	   newScreenSize.ws_row != screenSize.ws_row)
		memcpy(&screenSize, &newScreenSize, sizeof(struct winsize));

	textAttribute = ResetAll;
	foreground = White;
	background = Black;
	setTextStyle();
	eraseScreen();

	double now = getTime();
	frameDuration = now-lastTime;
	lastTime = now;
}
