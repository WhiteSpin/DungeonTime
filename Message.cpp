#include "Message.h"

std::vector<std::unique_ptr<Message>> Message::list;

Message::Message(const std::string& _text)
    :lifeTime(5.0), text(_text) {
}

void Message::doFrame() {
    for(int64_t i = list.size()-1; i >= 0; ) {
        Message* msg = list[i].get();
        if (list.size() - i < 11)
            printf("%s\n", msg->text.c_str());
            msg->lifeTime -= System::frameDuration;

            if(msg->lifeTime > 0.0)
                --i;
        else{
            list.erase(list.begin() + i);
            if(i == 0)
                break;
        }
    }
}

void Message::push(const std::string& text) {
    list.push_back(std::unique_ptr<Message>(new Message(text)));
}
