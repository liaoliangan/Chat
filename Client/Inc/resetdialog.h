//
// Created by 33717 on 2026/1/16.
//

#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include"global.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class ResetDialog;
}

QT_END_NAMESPACE

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget* parent = nullptr);
    ~ResetDialog() override;

private slots:
    void on_return_btn_clicked();
    void on_varify_btn_clicked();
    void slot_reset_mod_finish(LA::ReqId id, QString res, LA::ErrorCodes err);
    void on_sure_btn_clicked();
signals:
    void sigSwitchLogin();

private:
    Ui::ResetDialog* ui;
    QMap<LA::TipErr, QString> _tip_errs;
    QMap<LA::ReqId, std::function<void(const QJsonObject&)>> _handlers;
    void initHandlers();
    bool checkUserValid();
    bool checkPassValid();
    bool checkVarifyValid();
    bool checkEmailValid();
    void showTip(QString str, bool b_ok);
    void AddTipErr(LA::TipErr te, QString tips);
    void DelTipErr(LA::TipErr te);
};


#endif //RESETDIALOG_H
