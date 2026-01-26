//
// Created by 33717 on 2026/1/19.
//

#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>

class ChatUserList : public QListWidget
{
    Q_OBJECT

public:
    ChatUserList(QWidget* parent = nullptr);

protected:
    /**
     * @brief
     * @param watched 发生事件的对象指针，指向被监控事件过滤器监听的对象
     * @param event 具体的事件对象指针
     * @return
     */
    bool eventFilter(QObject* watched, QEvent* event) override;

signals:
    void sig_loading_chat_user();
private:
    bool _load_pending;
};


#endif //CHATUSERLIST_H
