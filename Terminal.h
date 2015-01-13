#include "Core.h"

class Terminal {
    public:
    Terminal();
    void setCursorPosition(uint64_t posX, uint64_t posY);
    uint64_t handleKeyboard(uint64_t bufferSize, uint8_t* buffer);
    bool isCSI(uint8_t* buffer);
};

extern Terminal term;
