//
// Created by 33717 on 2026/1/21.
//

#ifndef TEXTCHATMSG_H
#define TEXTCHATMSG_H

#include<QString>
#include<QJsonArray>
#include <QJsonObject>
#include"TextChatData.h"

struct TextChatMsg{
    TextChatMsg(int fromuid, int touid, QJsonArray arrays):
        _from_uid(fromuid),_to_uid(touid){
        for(auto  msg_data : arrays){
            auto msg_obj = msg_data.toObject();
            auto content = msg_obj["content"].toString();
            auto msgid = msg_obj["msgid"].toString();
            auto msg_ptr = std::make_shared<TextChatData>(msgid, content,fromuid, touid);
            _chat_msgs.push_back(msg_ptr);
        }
    }
    int _to_uid;
    int _from_uid;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};

#endif //TEXTCHATMSG_H
