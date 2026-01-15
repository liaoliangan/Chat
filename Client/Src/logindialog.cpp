//
// Created by 33717 on 2026/1/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginDialog.h" resolved

#include "logindialog.h"
#include "ui_LoginDialog.h"
#include "global.h"

LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->reg_btn, &QPushButton::clicked, this, [this]()
    {
        emit switchRegister();
    });
}

LoginDialog::~LoginDialog()
{
    COUT << "LoginDialog::~LoginDialog()" << ENDL;
    delete ui;
}
