//
// Created by 33717 on 2026/1/20.
//

#ifndef ADDUSERITEM_H
#define ADDUSERITEM_H

#include <QWidget>
#include "ListItemBase.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class AddUserItem;
}

QT_END_NAMESPACE

class AddUserItem :public ListItemBase
{
    Q_OBJECT

public:
    explicit AddUserItem(QWidget* parent = nullptr);
    ~AddUserItem() override;
    QSize sizeHint() const override;

private:
    Ui::AddUserItem* ui;
};


#endif //ADDUSERITEM_H
