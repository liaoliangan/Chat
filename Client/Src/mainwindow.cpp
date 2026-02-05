//
// Created by 33717 on 2026/1/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"

#include "TcpMgr.h"
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

    //创建和注册消息连接
    connect(__loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(__loginDialog, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_switch_chatdlg, this, &MainWindow::SlotSwitchChatDialog);
    connect(__loginDialog,&LoginDialog::switchQRCodeDialog,this,&MainWindow::SlotSwitchQRCodeDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    __registerDialog = new RegisterDialog(this);
    __registerDialog->setWindowTitle("register");
    __registerDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    __registerDialog->hide();
    connect(__registerDialog, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);

    setCentralWidget(__registerDialog);
    __loginDialog->hide();
    __registerDialog->show();
}

void MainWindow::SlotSwitchLogin()
{
    __loginDialog = new LoginDialog(this);
    __loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(__loginDialog);
    __registerDialog->hide();
    connect(__loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(__loginDialog, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    __resetDialog = new ResetDialog(this);
    __resetDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(__resetDialog);

    __loginDialog->hide();
    __resetDialog->show();
    //注册返回登录信号和槽函数
    connect(__resetDialog, &ResetDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLoginFromResetDialog);
}

void MainWindow::SlotSwitchChatDialog()
{
    __chatDialog = new ChatDialog(this);
    __chatDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(__chatDialog);
    __chatDialog->show();
    __loginDialog->hide();
    this->setMinimumSize(QSize(1050, 900));
    // this->setMaximumSize(QSize(1050, 900));
    this->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
}

void MainWindow::SlotSwitchLoginFromResetDialog()
{
    __loginDialog = new LoginDialog(this);
    __loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(__loginDialog);

    __resetDialog->hide();
    __loginDialog->show();
    connect(__loginDialog, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    connect(__loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(__loginDialog,&LoginDialog::switchQRCodeDialog,this,&MainWindow::SlotSwitchQRCodeDialog);
}

void MainWindow::SlotSwitchQRCodeDialog()
{
    __qrCodeDialog = new qrCodeDialog(this);
    __qrCodeDialog->show();
    connect(__qrCodeDialog,&qrCodeDialog::switchChatDialog,this,&MainWindow::SlotChatDialogFromQRCode);
    connect(__qrCodeDialog,&qrCodeDialog::qrCodeFailed,this,&MainWindow::SlotSwitchLoginFromQRCodeFailed);
}

void MainWindow::SlotChatDialogFromQRCode()
{
    __chatDialog = new ChatDialog(this);
    __chatDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(__chatDialog);
    __chatDialog->show();
    __loginDialog->hide();
    this->setMinimumSize(QSize(1050, 900));
    this->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
}

void MainWindow::SlotSwitchLoginFromQRCodeFailed()
{
    __qrCodeDialog->hide();
}
