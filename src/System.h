#include "Core.h"

namespace System {
    extern enum TextAttribute {
        ResetAll = 0,
        Bold = 1,
        Faint = 2,
        Italic = 3,
        Underline = 4,
        BlinkSlow = 5,
        BlinkRapid = 6,
        Inverse = 7,
        Hide = 8,
        CrossedOut = 9,
        DefaultFont = 10,
        AlternateFont = 11,
        DoubleUnderline = 21,
        ResetColorAndIntensity = 22,
        NoItalic = 23,
        NoUnderline = 24,
        NoBlink = 25,
        ResetInverse = 26,
        Reveal = 27,
        ResetCrossedOut = 28
    } textAttribute;

    extern enum Color {
        Black = 0,
        Red = 1,
        Green = 2,
        Yellow = 3,
        Blue = 4,
        Magenta = 5,
        Cyan = 6,
        White = 7,
        Default = 9
    } foreground, background;

    void init();
    void terminate();
    void eraseScreen();
    void setCursorPosition(uint64_t posX, uint64_t posY);
    void setTextStyle();
    void renderRightAlignedText(uint64_t row, const char* text);
    uint64_t handleKeyboard(uint64_t bufferSize, uint8_t* buffer);
    bool isCSI(uint8_t* buffer);
    double getTime();
    void doFrame();

    extern double frameDuration;
    extern struct termios original;
    extern struct winsize screenSize;
};
