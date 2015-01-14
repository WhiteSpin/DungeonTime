#include "Message.h"

std::vector<std::unique_ptr<Message>> messages;

Message::Message(const char* _text)
    :lifeTime(5.0), text(_text) {
}

void Message::doFrame() {
    printf("%s %f\n", text.c_str(), lifeTime);
    lifeTime -= System::frameDuration;
}
