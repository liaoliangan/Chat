//
// Created by 33717 on 2026/1/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RegisterDialog.h" resolved

#include "registerdialog.h"
#include"global.h"
#include <QStyle>

#include "HttpMgr.h"
#include "ui_RegisterDialog.h"


RegisterDialog::RegisterDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->email_Edit->setEchoMode(QLineEdit::Normal);
    ui->pass_Edit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->confirm_Edit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->err_tip->setProperty("state", "normal"); //设置状态
    LA::repolish(ui->err_tip);

    connect(HttpMgr::getInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::showTip(QString str, bool b_ok)
{
    if (b_ok)
    {
        ui->err_tip->setProperty("state", "normal");
    }
    else
    {
        ui->err_tip->setProperty("state", "error");
    }
    ui->err_tip->setText(str);
    LA::repolish(ui->err_tip);
}

void RegisterDialog::initHttpHandlers()
{
    //注册获取验证码回包的逻辑
    __handlers.insert(LA::ReqId::ID_GET_VARIFY_CODE, [this](QJsonObject obj)
    {
        LA::ErrorCodes err = static_cast<LA::ErrorCodes>(obj["error"].toInt());
        if (err != LA::ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = obj["email"].toString();
        showTip(tr("已发送验证码"), true);
        COUT << "email is " << email.toStdString() << ENDL;
    });
}

void RegisterDialog::on_get_code_clicked()
{
    auto email = ui->email_Edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"); //匹配email
    bool match = regex.match(email).hasMatch();
    if (match)
    {
        //匹配成功，发送http验证码
        showTip(tr("已发送验证码"), match);
    }
    else
    {
        //未匹配
        showTip(tr("邮箱地址不正确"), match);
    }
}

void RegisterDialog::slot_reg_mod_finish(LA::ReqId id, QString res, LA::ErrorCodes err)
{
    if (err != LA::ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"), false);
        return;
    }

    //解析JSON字符串，res转为QByteArray
    const QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    if (doc.isNull())
    {
        showTip(tr("JSON解析错误"), false);
        return;
    }
    if (!doc.isObject())
    {
        showTip(tr("JSON格式错误"), false);
    }

    QJsonObject obj = doc.object();
    __handlers[id](obj);
    return;
}
