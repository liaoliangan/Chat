//
// Created by 33717 on 2026/1/9.
//

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui
{
    class LoginDialog;
}

QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() override;
signals:
    void switchRegister();

private:
    Ui::LoginDialog* ui;
};


#endif //LOGINDIALOG_H
