#include "Message.h"

Message::Message(const char* _text)
    :lifeTime(5.0), text(_text) {
}

void Message::render() {
    printf("%s %f\n", text.c_str(), lifeTime);
    lifeTime -= 0.05;
}

std::vector<std::unique_ptr<Message>> messages;
