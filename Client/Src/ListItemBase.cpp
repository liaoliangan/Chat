//
// Created by 33717 on 2026/1/19.
//

#include "ListItemBase.h"

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
