//
// Created by 33717 on 2026/1/20.
//

#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H

#include<QFrame>
#include<QHBoxLayout>
#include"global.h"

//我们的消息分为几种，文件，文本，图片等。所以先实现BubbleFrame作为基类
class BubbleFrame : public QFrame
{
    Q_OBJECT

public:
    BubbleFrame(ChatRole role, QWidget* parent = nullptr);
    void setMargin(int margin);
    //inline int margin(){return margin;}
    void setWidget(QWidget* w);

protected:
    void paintEvent(QPaintEvent* e);

private:
    QHBoxLayout* m_pHLayout;
    ChatRole m_role;
    int m_margin;
};


#endif //BUBBLEFRAME_H
