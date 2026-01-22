//
// Created by 33717 on 2026/1/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GroupTipItem.h" resolved

#include "grouptipitem.h"
#include "ui_GroupTipItem.h"


GroupTipItem::GroupTipItem(QWidget *parent) :ListItemBase (parent),_tip(""),
    ui(new Ui::GroupTipItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::GROUP_TIP_ITEM);
}

GroupTipItem::~GroupTipItem()
{
    delete ui;
}


QSize GroupTipItem::sizeHint() const
{
    return QSize(250, 25); // 返回自定义的尺寸
}

void GroupTipItem::SetGroupTip(QString str)
{
    ui->label->setText(str);
}
