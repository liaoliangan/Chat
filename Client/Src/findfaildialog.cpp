//
// Created by 33717 on 2026/1/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FindFailDialog.h" resolved

#include "findfaildialog.h"
#include "ui_FindFailDialog.h"


FindFailDialog::FindFailDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::FindFailDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("FindFailDlg");
    ui->fail_sure_btn->SetState("normal", "hover", "press");
    this->setModal(true);
}

FindFailDialog::~FindFailDialog()
{
#ifdef LADEBUG
    qDebug() << "Find FailDlg destruct";
#endif

    delete ui;
}

void FindFailDialog::on_fail_sure_btn_clicked()
{
    this->hide();
}
