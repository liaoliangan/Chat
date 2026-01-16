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
    QDialog(parent), ui(new Ui::RegisterDialog), _countdown(5)
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

    ui->err_tip->clear();

    connect(ui->user_Edit, &QLineEdit::editingFinished, this, [this]()
    {
        checkUserValid();
    });

    connect(ui->email_Edit, &QLineEdit::editingFinished, this, [this]()
    {
        checkEmailValid();
    });

    connect(ui->pass_Edit, &QLineEdit::editingFinished, this, [this]()
    {
        checkPassValid();
    });

    connect(ui->confirm_Edit, &QLineEdit::editingFinished, this, [this]()
    {
        checkConfirmValid();
    });

    connect(ui->varify_Edit, &QLineEdit::editingFinished, this, [this]()
    {
        checkVarifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor); //设置鼠标样式(小手的样子)
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->SetState("unvisible", "unvisible_hover", "", "visible",
                               "visible_hover", "");

    ui->confirm_visible->SetState("unvisible", "unvisible_hover", "", "visible",
                                  "visible_hover", "");

    connect(ui->pass_visible, &ClickedLabel::clicked, this, [this]()
    {
        const auto state = ui->pass_visible->GetCurState();
        if (state == LA::ClickLbState::Normal)
        {
            ui->pass_Edit->setEchoMode(QLineEdit::Password);
        }
        else
        {
            ui->pass_Edit->setEchoMode(QLineEdit::Normal);
        }
        COUT << "pass Label was clicked!" << ENDL;
    });

    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]()
    {
        auto state = ui->confirm_visible->GetCurState();
        if (state == LA::ClickLbState::Normal)
        {
            ui->confirm_Edit->setEchoMode(QLineEdit::Password);
        }
        else
        {
            ui->confirm_Edit->setEchoMode(QLineEdit::Normal);
        }
        COUT << "confirm Label was clicked!" << ENDL;
    });

    _countdown_timer = new QTimer(this);
    connect(_countdown_timer, &QTimer::timeout, this, [this]()
    {
        if (_countdown == 0)
        {
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回").arg(_countdown);
        ui->tip_label->setText(str);
    });
    connect(ui->return_btn, &QPushButton::clicked, this, &RegisterDialog::on_return_btn_clicked);
    connect(ui->cancel_btn, &QPushButton::clicked, this, &RegisterDialog::sigSwitchLogin);
}

RegisterDialog::~RegisterDialog()
{
    COUT << "RegisterDialog::~RegisterDialog()" << ENDL;
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
        COUT << "user uuid is " << obj["uid"].toString().toStdString() << ENDL;
        COUT << "email is " << email.toStdString() << ENDL;
        ChangeTipPage();
    });
}

void RegisterDialog::AddTipErr(LA::TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::DelTipErr(LA::TipErr te)
{
    _tip_errs.remove(te);
    if (_tip_errs.empty())
    {
        ui->err_tip->clear();
        return;
    }

    showTip(_tip_errs.first(), false);
}

bool RegisterDialog::checkUserValid()
{
    if (ui->user_Edit->text() == "")
    {
        AddTipErr(LA::TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    DelTipErr(LA::TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_Edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if (!match)
    {
        //提示邮箱不正确
        AddTipErr(LA::TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }

    DelTipErr(LA::TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPassValid()
{
    auto pass = ui->pass_Edit->text();

    if (pass.length() < 6 || pass.length() > 15)
    {
        //提示长度不准确
        AddTipErr(LA::TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if (!match)
    {
        //提示字符非法
        AddTipErr(LA::TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }

    DelTipErr(LA::TipErr::TIP_PWD_ERR);

    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    if (ui->pass_Edit->text() != ui->confirm_Edit->text())
    {
        AddTipErr(LA::TipErr::TIP_CONFIRM_ERR, tr("两次输入的密码不一致"));
        return false;
    }
    DelTipErr(LA::TipErr::TIP_CONFIRM_ERR);
    return true;
}

bool RegisterDialog::checkVarifyValid()
{
    auto pass = ui->varify_Edit->text();
    if (pass.isEmpty())
    {
        AddTipErr(LA::TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }

    DelTipErr(LA::TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::ChangeTipPage() const
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    //启动定时器，时间间隔设为1s
    _countdown_timer->start();
    _countdown_timer->setInterval(1000);
}

void RegisterDialog::on_get_code_clicked()
{
    const auto email = ui->email_Edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"); //匹配email
    if (bool match = regex.match(email).hasMatch())
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

    //发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user_name"] = ui->user_Edit->text();
    json_obj["email"] = ui->email_Edit->text();
    json_obj["passwd"] = LA::md5Hash(ui->pass_Edit->text());
    json_obj["confirm"] = LA::md5Hash(ui->confirm_Edit->text());
    json_obj["varifycode"] = ui->varify_Edit->text();
    HttpMgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
                                        json_obj, LA::ReqId::ID_REG_USER, LA::Modules::REGISTERMOD);
}

void RegisterDialog::on_return_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
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
