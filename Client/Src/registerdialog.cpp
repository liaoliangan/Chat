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
    connect(ui->sure_btn, &QPushButton::clicked, this, &RegisterDialog::on_sure_btn_clicked);
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

    __handlers.insert(LA::ReqId::ID_REG_USER, [this](QJsonObject obj)
    {
        LA::ErrorCodes err = static_cast<LA::ErrorCodes>(obj["error"].toInt());
        if (err != LA::ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"), false);
            return;
        }
        const QString email = obj["email"].toString();
        showTip(tr("用户注册成功"), true);
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
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_varifycode"), json_obj,
                                            LA::ReqId::ID_GET_VARIFY_CODE, LA::Modules::REGISTERMOD);
    }
    else
    {
        //未匹配
        showTip(tr("邮箱地址不正确"), match);
    }
}

void RegisterDialog::on_sure_btn_clicked()
{
    if (ui->user_Edit->text() == "")
    {
        showTip(tr("用户名不能为空"), false);
        return;
    }

    if (ui->email_Edit->text() == "")
    {
        showTip(tr("邮箱不能为空"), false);
        return;
    }

    if (ui->pass_Edit->text() == "")
    {
        showTip(tr("密码不能为空"), false);
        return;
    }

    if (ui->confirm_Edit->text() == "")
    {
        showTip(tr("确认密码不能为空"), false);
        return;
    }

    if (ui->confirm_Edit->text() != ui->pass_Edit->text())
    {
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }

    if (ui->varify_Edit->text() == "")
    {
        showTip(tr("验证码不能为空"), false);
        return;
    }

    //day11 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_Edit->text();
    json_obj["email"] = ui->email_Edit->text();
    json_obj["passwd"] = ui->pass_Edit->text();
    json_obj["confirm"] = ui->confirm_Edit->text();
    json_obj["varifycode"] = ui->varify_Edit->text();
    HttpMgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
                                        json_obj, LA::ReqId::ID_REG_USER, LA::Modules::REGISTERMOD);
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
}
