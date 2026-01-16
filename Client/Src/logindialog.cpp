//
// Created by 33717 on 2026/1/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginDialog.h" resolved

#include "logindialog.h"
#include "ui_LoginDialog.h"
#include <qdebug.h>
#include <QPainter>
#include <QPainterPath>
#include "HttpMgr.h"

LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);
    // initHead();
    initHttpHandlers();

    //连接登录回包信号
    connect(HttpMgr::getInstance().get(), &HttpMgr::sig_login_mod_finish, this, &LoginDialog::slot_login_mod_finish);
}

LoginDialog::~LoginDialog()
{
    COUT << "LoginDialog::~LoginDialog()" << ENDL;
    delete ui;
}

void LoginDialog::slot_forget_pwd()
{
    COUT << "slot forget pwd" << ENDL;
    emit switchReset();
}

void LoginDialog::on_login_btn_clicked()
{
    qDebug() << "login btn clicked";
    if (!checkUserValid())
        return;

    if (!checkPwdValid())
        return;
    enableBtn(false);
    auto email = ui->email_Edit->text();
    auto pwd = ui->pass_Edit->text();
    //发送http请求登录
    QJsonObject json_obj;
    json_obj["user"] = email;
    json_obj["passwd"] = LA::md5Hash(pwd);
    HttpMgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_login"),
                                        json_obj, LA::ReqId::ID_LOGIN_USER, LA::Modules::LOGINMOD);
}

void LoginDialog::slot_login_mod_finish(LA::ReqId id, QString res, LA::ErrorCodes err)
{
    if (err != LA::ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"), false);
        return;
    }
    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if (jsonDoc.isNull())
    {
        showTip(tr("json解析错误"), false);
        return;
    }
    //json解析错误
    if (!jsonDoc.isObject())
    {
        showTip(tr("json解析错误"), false);
        return;
    }
    //调用对应的逻辑,根据id回调。
    _handlers[id](jsonDoc.object());
}

bool LoginDialog::checkUserValid()
{
    auto email = ui->email_Edit->text();
    if (email.isEmpty())
    {
        qDebug() << "email empty ";
        AddTipErr(LA::TipErr::TIP_EMAIL_ERR, tr("邮箱不能为空"));
        return false;
    }
    DelTipErr(LA::TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pwd = ui->pass_Edit->text();
    if (pwd.length() < 6 || pwd.length() > 15)
    {
        qDebug() << "Pass length invalid";
        AddTipErr(LA::TipErr::TIP_PWD_ERR, tr("密码设置不合法"));
        return false;
    }
    DelTipErr(LA::TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::showTip(QString str, bool b_ok)
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

void LoginDialog::AddTipErr(LA::TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipErr(LA::TipErr te)
{
    _tip_errs.remove(te);
    if (_tip_errs.empty())
    {
        ui->err_tip->clear();
        return;
    }

    showTip(_tip_errs.first(), false);
}

void LoginDialog::enableBtn(bool enable)
{
    ui->login_btn->setEnabled(enable);
    ui->reg_btn->setEnabled(enable);
}

void LoginDialog::initHttpHandlers()
{
    //注册获取登录回包逻辑
    _handlers.insert(LA::ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj)
    {
        int error = jsonObj["error"].toInt();
        if (error != static_cast<int>(LA::ErrorCodes::SUCCESS))
        {
            showTip(tr("参数错误"), false);
            enableBtn(true);
            return;
        }
        auto email = jsonObj["email"].toString();

        //发送信号通知tcpMgr发送长连接
        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;
        qDebug() << "email is " << email << " uid is " << si.Uid << " host is " << si.Host << " port is " << si.Port <<
            " Token is " << si.Token;
        emit sig_connect_tcp(si);
    });
}

void LoginDialog::initHead()
{
    QPixmap originalPixmap(":/image/head_1.png");
    if (originalPixmap.isNull())
    {
        qDebug() << "Error: Failed to load image :/image/head_1.png";
        // 设置默认图片或退出函数
        return;
    }
    //设置图片自动缩放
    qDebug() << originalPixmap.size() << ui->head_label->size();

    //将原始头像图片按比例缩放到适合显示在 head_label 控件中的尺寸，同时保持图片质量和原始比例。这是在为后续的圆角处理做准备。
    originalPixmap = originalPixmap.scaled(ui->head_label->size(),
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //创建一个和原始图片大小相同的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent); //设置背景透明

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); //启用抗锯齿，使圆角更平滑
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //使用QPainterPath设置圆角
    QPainterPath path;
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10); //最后两个参数为圆角半径
    painter.setClipPath(path);

    //将原始图片绘制成rounderPixmap上
    painter.drawPixmap(0, 0, originalPixmap);

    //设置绘制好的圆角图片到QLabel上
    ui->head_label->setPixmap(roundedPixmap);
}
