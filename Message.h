#include "Terminal.h"

class Message {
    public:
    std::string text;
    float lifeTime;
    Message(const char* text);
    void render();
};

extern std::vector<std::unique_ptr<Message>> messages;
