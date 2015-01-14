#include "System.h"

class Message {
    public:
    std::string text;
    float lifeTime;
    Message(const char* text);
    static void doFrame();
};

extern std::vector<std::unique_ptr<Message>> messages;
