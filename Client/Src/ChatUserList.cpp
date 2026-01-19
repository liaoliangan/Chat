//
// Created by 33717 on 2026/1/19.
//

#include "ChatUserList.h"
ChatUserList::ChatUserList(QWidget *parent):QListWidget(parent)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏垂直滚动条
    // 安装事件过滤器
    //viewport():返回QListWidget的视口部件指针(实际显示的区域)
    this->viewport()->installEventFilter(this);
}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
#ifdef LADEBUG
            qDebug()<<"show scrollbar";
#endif
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            //Qt::ScrollBarAsNeeded 策略只有在内容超出可视区域时才会显示滚动条
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
#ifdef LADEBUG
            qDebug()<<"hide scrollbar";
#endif
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        // 检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10; // 每页加载的联系人数量

        if (maxScrollValue - currentValue <= 0) {
            // 滚动到底部，加载新的联系人
#ifdef LADEBUG
            qDebug()<<"load more chat user";
#endif

            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_chat_user();
        }
        return true; // 停止事件传递
    }
    return QListWidget::eventFilter(watched, event);
}