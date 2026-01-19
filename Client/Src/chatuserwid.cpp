//
// Created by 33717 on 2026/1/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatUserWid.h" resolved

#include "chatuserwid.h"
#include "ui_ChatUserWid.h"


ChatUserWid::ChatUserWid(QWidget* parent) :
    ListItemBase(parent),
    ui(new Ui::ChatUserWid)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatUserWid::~ChatUserWid()
{
    delete ui;
}

QSize ChatUserWid::sizeHint() const
{
    return QSize(250, 70); // 返回自定义的尺寸
}

void ChatUserWid::SetInfo(QString name, QString head, QString msg)
{
    _name = name;
    _head = head;
    _msg = msg;
    QString default_pixmap = ":/image/login_ioc.png";
    // 加载图片
    QPixmap pixmap;
    if (!head.isEmpty())
    {
        if (!pixmap.load(head))
        {
            qWarning() << "Failed to load image:" << head;
            pixmap.load(default_pixmap);
        }
    }
    else
    {
        pixmap.load(default_pixmap);
    }

    // 设置图片自动缩放
    ui->icon_label->setPixmap(pixmap.scaled(ui->icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_label->setScaledContents(true);

    ui->user_name_libel->setText(_name);
    ui->user_chat_label->setText(_msg);
}
