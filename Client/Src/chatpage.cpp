//
// Created by 33717 on 2026/1/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatPage.h" resolved

#include "chatpage.h"
#include "ui_ChatPage.h"


ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::ChatPage) {
    ui->setupUi(this);
}

ChatPage::~ChatPage() {
    delete ui;
}
