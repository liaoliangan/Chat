//
// Created by 33717 on 2026/1/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FindSuccessDialog.h" resolved

#include "findsuccessdialog.h"

#include <QDir>

#include "applyfriend.h"
#include "ui_FindSuccessDialog.h"


FindSuccessDialog::FindSuccessDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::FindSuccessDialog), _parent(parent)
{
    ui->setupUi(this);
    //设置对话框标题
    setWindowTitle("添加");
    //隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    //获取当前app程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    QDir dir(app_path);
    dir.cdUp();
    app_path = dir.path();
    //测试路径，需要去添加static文件夹---Client/static
    QString pix_path = QDir::toNativeSeparators(
        app_path + QDir::separator() + "static" + QDir::separator() + "head_1.jpg");
#ifdef LADEBUG
    qDebug() << "pix_path is " << pix_path;
#endif
    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->head_label->size(),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_label->setPixmap(head_pix);
    ui->add_friend_btn->SetState("normal", "hover", "press");
    this->setModal(true); //模态对话框
}

FindSuccessDialog::~FindSuccessDialog()
{
#ifdef LADEBUG
    qDebug() << "~FindSuccessDialog()";
#endif

    delete ui;
}

auto FindSuccessDialog::SetSearchInfo(std::shared_ptr<SearchInfo> si) -> void
{
#ifdef LADEBUG
    qDebug() << "SetSearchInfo(): " << *(si.get());
#endif

    ui->name_label->setText(si->_name);
    _si = si;
}

auto FindSuccessDialog::on_add_friend_btn_clicked() -> void
{
    this->hide();
    //弹出加好友界面
#ifdef LADEBUG
    qDebug()<<"on_add_friend_btn_clicked()";
#endif

    auto applyFriend = new ApplyFriend(_parent);
#ifdef LADEBUG
    qDebug()<<"applyFriend is "<<applyFriend;
#endif
#ifdef LADEBUG
    qDebug()<<"si: "<<*(_si.get());
#endif
    applyFriend->SetSearchInfo(_si);
#ifdef LADEBUG
    qDebug()<<"applyFriend->SetSearchInfo(_si)";
#endif

    applyFriend->setModal(true);
    applyFriend->show();
}
