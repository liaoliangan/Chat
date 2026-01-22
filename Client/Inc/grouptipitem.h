//
// Created by 33717 on 2026/1/22.
//

#ifndef GROUPTIPITEM_H
#define GROUPTIPITEM_H

#include <QWidget>
#include"ListItemBase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GroupTipItem; }
QT_END_NAMESPACE

class GroupTipItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit GroupTipItem(QWidget *parent = nullptr);
    ~GroupTipItem();
    QSize sizeHint() const override;
    void SetGroupTip(QString str);
private:
    QString _tip;
    Ui::GroupTipItem *ui;
};


#endif //GROUPTIPITEM_H
