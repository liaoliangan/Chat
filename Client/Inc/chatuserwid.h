//
// Created by 33717 on 2026/1/19.
//

#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include"ListItemBase.h"
#include "userdata.h"
QT_BEGIN_NAMESPACE

namespace Ui
{
    class ChatUserWid;
}

QT_END_NAMESPACE

class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget* parent = nullptr);
    ~ChatUserWid();

    QSize sizeHint() const override;

    void SetInfo(std::shared_ptr<UserInfo> user_info);
    void SetInfo(std::shared_ptr<FriendInfo> friend_info);
    void ShowRedPoint(bool bshow);
    std::shared_ptr<UserInfo> GetUserInfo();
    void updateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs);
private:
    Ui::ChatUserWid* ui;
    std::shared_ptr<UserInfo> _user_info;
};


#endif //CHATUSERWID_H
