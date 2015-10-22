#include "Level.h"

std::ofstream logFile;
System::TextAttribute System::textAttribute;
System::Color System::foreground, System::background;
double System::frameDuration, lastTime;
struct termios System::termiosOld, System::termiosNew;
struct winsize System::screenSize;
const std::string CSI = "\33["; // "\e["

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

void System::setCursorPosition(uint64_t x, uint64_t y) {
	std::cout << CSI << (y+1) << ';' << (x+1) << 'f';
}

void System::eraseScreen() {
	// std::cout << CSI << "2J";
	for(uint64_t y = 0; y < screenSize.ws_row; ++y) {
		setCursorPosition(0, y);
        std::cout << CSI << "2K";
	}
}

void System::setTextStyle() {
    std::cout << CSI << textAttribute << ';' << (foreground+30) << ';' << (background+40) << 'm';
}

void System::pollKeyboard(std::function<uint64_t(bool, uint64_t, const char*)> callback) {
	fd_set readset;
	struct timeval tv;
	FD_ZERO(&readset);
	FD_SET(STDIN_FILENO, &readset);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	select(STDIN_FILENO+1, &readset, NULL, NULL, &tv);
	if(FD_ISSET(STDIN_FILENO, &readset)) {
        char buffer[16];
        uint64_t pos = 0, readBytes = read(STDIN_FILENO, buffer, sizeof(buffer));
        while(pos < readBytes) {
            bool special = (readBytes-pos >= 2 && CSI.compare(0, 2, buffer+pos, 2) == 0);
            if(special) pos += 2;
            pos += callback(special, readBytes-pos, buffer+pos);
        }
	}
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
