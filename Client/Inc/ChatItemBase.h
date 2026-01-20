//
// Created by 33717 on 2026/1/20.
//

#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include<QWidget>
#include<QGridLayout>
#include<QLabel>
#include<global.h>

class BubbleFrame;

class ChatItemBase : public QWidget
{
    Q_OBJECT

public:
    ChatItemBase(ChatRole role, QWidget* parent = nullptr);
    void setUserName(const QString& name) const;
    void setUserIcon(const QPixmap& icon) const;

    /**
     * @brief 气泡框可以看作：绿色的背景+文本==widget，因此可以把widget单独拿出来，放在气泡框中方便发图片、视频，而不只是文字
     * @param widget
     */
    void setWidget(QWidget* widget);

private:
    ChatRole m_role;
    mutable QLabel* m_pNameLabel;
    mutable QLabel* m_pIconLabel;
    QWidget* m_pBubble;
};


#endif //CHATITEMBASE_H
