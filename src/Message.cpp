#include "Level.h"

std::vector<std::unique_ptr<Message>> Message::list;

Message::Message(const std::string& _text)
    :lifeTime(50.0), text(_text) {
}

void Message::doFrame() {
    int64_t row = level->height + 1;

    for(int64_t i = list.size()-1; i >= 0; --i) {
        Message* msg = list[i].get();

        if(row ++ < System::screenSize.ws_row) {
            System::setCursorPosition(0, row);
            fwrite(msg->text.c_str(), 1, msg->text.size(), stdout);
        }

        msg->lifeTime -= System::frameDuration;
        if(msg->lifeTime <= 0.0) {
            list.erase(list.begin() + i);
            if(i == 0)
                break;
        }
    }
}

void Message::push(const std::string& text) {
    list.push_back(std::unique_ptr<Message>(new Message(text)));
}
