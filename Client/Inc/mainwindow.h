//
// Created by 33717 on 2026/1/9.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
#include "chatdialog.h"
#include "qrcodedialog.h"
QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();
    void SlotSwitchChatDialog();
    void SlotSwitchLoginFromResetDialog();
    void SlotSwitchQRCodeDialog();
    void SlotChatDialogFromQRCode();
    void SlotSwitchLoginFromQRCodeFailed();
private:
    Ui::MainWindow* ui;
    LoginDialog* __loginDialog;
    RegisterDialog* __registerDialog;
    ResetDialog* __resetDialog;
    ChatDialog* __chatDialog;
    qrCodeDialog* __qrCodeDialog;
};


#endif //MAINWINDOW_H
