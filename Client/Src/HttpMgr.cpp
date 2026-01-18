//
// Created by 33717 on 2026/1/10.
//

#include "HttpMgr.h"

HttpMgr::HttpMgr()
{
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(const QUrl& url, QJsonObject json, LA::ReqId req_id, LA::Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    auto self = shared_from_this(); //创建伪闭包，因为网络发送是异步，防止函数结束时对象被析构

    QNetworkReply* reply = _manager.post(request, data);
    connect(reply, &QNetworkReply::finished, [self,reply,req_id,mod]()-> void
    {
        //捕获self，防止回调函数执行时对象本身被析构
        if (reply->error() != QNetworkReply::NoError)
        {
            COUT << reply->errorString().toStdString() << ENDL;
            emit self->sig_http_finish(req_id, "", LA::ErrorCodes::RPCFAILED, mod);
            reply->deleteLater(); //等到不用的时候，qt自动回收
            return;
        }

        QString res = reply->readAll();
        //发送信号通知完成
        emit self->sig_http_finish(req_id, res, LA::ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
    });
}

void HttpMgr::slot_http_finish(LA::ReqId id, QString res, LA::ErrorCodes err, LA::Modules mod)
{
    if (mod == LA::Modules::REGISTERMOD) //注册模块
    {
        //发送信号通知指定模块
        emit sig_reg_mod_finish(id, res, err);
    }else if (mod==LA::Modules::RESETMOD)//重置密码模块
    {
        emit sig_reset_mod_finish(id,res,err);
    }else if (mod==LA::Modules::LOGINMOD)//登录模块
    {
        emit sig_login_mod_finish(id,res,err);
    }
}

HttpMgr::~HttpMgr() = default;
