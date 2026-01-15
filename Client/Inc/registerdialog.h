//
// Created by 33717 on 2026/1/9.
//

#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QJsonObject>

#include "global.h"


QT_BEGIN_NAMESPACE

namespace Ui
{
    class RegisterDialog;
}

QT_END_NAMESPACE

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget* parent = nullptr);
    ~RegisterDialog() override;

signals:
    void sigSwitchLogin();

private slots:
    void on_get_code_clicked();
    void on_sure_btn_clicked();
    void on_return_btn_clicked();
    void slot_reg_mod_finish(LA::ReqId id, QString res, LA::ErrorCodes err);

private:
    Ui::RegisterDialog* ui;
    QMap<LA::TipErr, QString> _tip_errs;
    QMap<LA::ReqId, std::function<void(const QJsonObject&)>> __handlers;

    void showTip(QString str, bool b_ok);
    void initHttpHandlers();
    void AddTipErr(LA::TipErr te, QString tips);
    void DelTipErr(LA::TipErr te);
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();
    void ChangeTipPage() const;
    QTimer* _countdown_timer;
    int _countdown;
};


#endif //REGISTERDIALOG_H
