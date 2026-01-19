//
// Created by 33717 on 2026/1/19.
//

#include "ListItemBase.h"

#include <QPainter>
#include <qstyleoption.h>

ListItemBase::ListItemBase(QWidget *parent) : QWidget(parent)
{

}

void ListItemBase::SetItemType(ListItemType itemType)
{
    _itemType = itemType;
}

ListItemType ListItemBase::GetItemType() const
{
    return _itemType;
}

void ListItemBase::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
