//
// Created by 33717 on 2026/1/19.
//

#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include"ListItemBase.h"

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

    void SetInfo(QString name, QString head, QString msg);

private:
    Ui::ChatUserWid* ui;
    QString _name;
    QString _head;
    QString _msg;
};


#endif //CHATUSERWID_H
