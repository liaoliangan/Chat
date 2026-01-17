//
// Created by 33717 on 2026/1/9.
//

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"
#include "ServerInfo.h"
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
    void switchReset();
    void sig_connect_tcp(ServerInfo);

private slots:
    void slot_forget_pwd();
    void on_login_btn_clicked();
    void slot_login_mod_finish(LA::ReqId id, QString res, LA::ErrorCodes err);
    void slot_tcp_con_finish(bool bsuccess);
    void slot_login_failed(int);
private:
    Ui::LoginDialog* ui;
    QString _token;
    int _uid;
    QMap<LA::TipErr, QString> _tip_errs;
    QMap<LA::ReqId, std::function<void(const QJsonObject&)>> _handlers;
    void initHead();
    bool checkUserValid();
    bool checkPwdValid();
    void showTip(const QString& str, bool b_ok) const;
    void AddTipErr(LA::TipErr te, QString tips);
    void DelTipErr(LA::TipErr te);
    void enableBtn(bool enable) const;
    void initHttpHandlers();
};

#endif //LOGINDIALOG_H
