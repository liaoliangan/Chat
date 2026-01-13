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

private slots:
    void on_get_code_clicked();
    void on_sure_btn_clicked();
    void slot_reg_mod_finish(LA::ReqId id, QString res, LA::ErrorCodes err);
private:
    Ui::RegisterDialog* ui;
    void showTip(QString str, bool b_ok);
    void initHttpHandlers();
    QMap<LA::ReqId, std::function<void(const QJsonObject&)>> __handlers;
};


#endif //REGISTERDIALOG_H
