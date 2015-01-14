#include "Message.h"

std::vector<std::unique_ptr<Message>> messages;

Message::Message(const char* _text)
    :lifeTime(5.0), text(_text) {
}

void Message::doFrame() {
    for(int64_t i = messages.size()-1; i >= 0; ) {
        Message* msg = messages[i].get();

        printf("%s %f\n", msg->text.c_str(), msg->lifeTime);
        msg->lifeTime -= System::frameDuration;

        if(msg->lifeTime > 0.0)
            --i;
        else{
            messages.erase(messages.begin() + i);
            if(i == 0)
                break;
        }
    }
}
