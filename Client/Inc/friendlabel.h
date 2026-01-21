//
// Created by 33717 on 2026/1/21.
//

#ifndef FRIENDLABEL_H
#define FRIENDLABEL_H

#include <QWidget>

#include "BubbleFrame.h"


QT_BEGIN_NAMESPACE

namespace Ui
{
    class FriendLabel;
}

QT_END_NAMESPACE

class FriendLabel : public QFrame
{
    Q_OBJECT

public:
    explicit FriendLabel(QWidget* parent = nullptr);
    ~FriendLabel() override;
    void SetText(QString text);
    int Width();
    int Height();
    QString Text();

private:
    Ui::FriendLabel* ui;
    QString _text;
    int _width;
    int _height;
signals:
    void sig_close(QString);

public slots:
    void slot_close();
};

#endif //FRIENDLABEL_H
