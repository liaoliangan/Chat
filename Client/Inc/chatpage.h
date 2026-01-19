//
// Created by 33717 on 2026/1/19.
//

#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include<QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatPage; }
QT_END_NAMESPACE

class ChatPage : public QWidget {
Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage() override;
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    Ui::ChatPage *ui;
};


#endif //CHATPAGE_H
