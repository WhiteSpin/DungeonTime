#include "System.h"

class Message {
    public:
    std::string text;
    float lifeTime;
    Message(const std::string& text);

    static std::vector<std::unique_ptr<Message>> list;
    static void doFrame();
    static void push(const std::string& text);
};
