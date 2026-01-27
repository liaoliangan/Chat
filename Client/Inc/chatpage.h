//
// Created by 33717 on 2026/1/19.
//

#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include<QPaintEvent>
#include "userdata.h"
QT_BEGIN_NAMESPACE

namespace Ui
{
    class ChatPage;
}

QT_END_NAMESPACE

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget* parent = nullptr);
    ~ChatPage() override;
    void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    void AppendChatMsg(std::shared_ptr<TextChatData> msg);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void on_send_btn_clicked();
signals:
    void sig_append_send_chat_msg(std::shared_ptr<TextChatData> msg);

private:
    Ui::ChatPage* ui;
    std::shared_ptr<UserInfo> _user_info;
};


#endif //CHATPAGE_H
