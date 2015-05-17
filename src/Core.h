#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <time.h>
#include "netLink.h" 

class Item;
class Level;

void initializeLog();
void writeToLog(const std::string &str);
void writeToLog(const char *str);
