//
// Created by 33717 on 2026/1/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FriendLabel.h" resolved

#include "friendlabel.h"
#include "ui_FriendLabel.h"


FriendLabel::FriendLabel(QWidget* parent) :
    QFrame(parent), ui(new Ui::FriendLabel)
{
    ui->setupUi(this);
    ui->close_label->SetState("normal", "hover", "pressed",
                              "selected_normal", "selected_hover", "selected_pressed");
    connect(ui->close_label, &ClickedLabel::clicked, this, &FriendLabel::slot_close);
}

FriendLabel::~FriendLabel()
{
    delete ui;
}

void FriendLabel::SetText(QString text)
{
    _text = text;
    ui->tip_label->setText(_text);
    ui->tip_label->adjustSize();

    QFontMetrics fontMetrics(ui->tip_label->font()); // 获取QLabel控件的字体信息
    auto textWidth = fontMetrics.horizontalAdvance(ui->tip_label->text()); // 获取文本的宽度
    auto textHeight = fontMetrics.height(); // 获取文本的高度

#ifdef LADEBUG
    qDebug() << " ui->tip_label.width() is " << ui->tip_label->width();
    qDebug() << " ui->close_label->width() is " << ui->close_label->width();
    qDebug() << " textWidth is " << textWidth;
#endif

    this->setFixedWidth(ui->tip_label->width() + ui->close_label->width() + 5);
    this->setFixedHeight(textHeight + 2);
#ifdef LADEBUG
    qDebug() << "  this->setFixedHeight " << this->height();
#endif

    _width = this->width();
    _height = this->height();
}

int FriendLabel::Width()
{
    return _width;
}

int FriendLabel::Height()
{
    return _height;
}

QString FriendLabel::Text()
{
    return _text;
}

void FriendLabel::slot_close()
{
    emit sig_close(_text);
}
