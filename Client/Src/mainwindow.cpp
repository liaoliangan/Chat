//
// Created by 33717 on 2026/1/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    __loginDialog = new LoginDialog(this);
    setCentralWidget(__loginDialog);
    __loginDialog->setWindowTitle("login");

    //设置Dialog属性，使dialog嵌入mainwindow
    __loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    __registerDialog = new RegisterDialog(this);
    __registerDialog->setWindowTitle("register");
    __registerDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    __registerDialog->hide();
    //创建和注册消息连接
    connect(__loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    setCentralWidget(__registerDialog);
    __loginDialog->hide();
    __registerDialog->show();
}
