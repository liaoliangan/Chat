//
// Created by 33717 on 2026/1/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatDialog.h" resolved

#include "chatdialog.h"
#include "ui_ChatDialog.h"


ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
}

ChatDialog::~ChatDialog() {
    delete ui;
}
