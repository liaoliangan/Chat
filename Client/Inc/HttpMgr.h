//
// Created by 33717 on 2026/1/10.
//

#ifndef HTTPMGR_H
#define HTTPMGR_H
#include "singalton.h"
#include <QUrl>
#include<QObject>
#include<QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

class HttpMgr : public QObject,
                public Singalton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT

public:
    ~HttpMgr();
    /**
     * @brief 发送http请求
     * @param url 请求的url
     * @param json 请求的json数据
     * @param req_id 具体模块中的具体功能的id
     * @param mod 模块id
     */
    void PostHttpReq(const QUrl& url, QJsonObject json, LA::ReqId req_id, LA::Modules mod);
private:
    friend class Singalton<HttpMgr>; //将基类声明为友元，使得Signalton可以访问HttpMgr的构造函数
    HttpMgr();
    QNetworkAccessManager _manager;
private slots:
    /**
     * @brief http请求完成后的槽函数
     * @param id 请求的id
     * @param res 响应结果
     * @param err 错误码
     * @param mod 模块id
     */
    void slot_http_finish(LA::ReqId id, QString res, LA::ErrorCodes err, LA::Modules mod);

signals:
    /**
     * @brief http请求完成后发送信号通知其他模块
     * @param id 请求的id
     * @param res 响应结果
     * @param err 错误码
     * @param mod 模块id
     */
    void sig_http_finish(LA::ReqId id, QString res, LA::ErrorCodes err, LA::Modules mod);

    /**
     * @brief 注册模块完成后发送信号
     * @param id 请求id
     * @param res 相应
     * @param err 错误码
     */
    void sig_reg_mod_finish(LA::ReqId id, QString res, LA::ErrorCodes err);
};


#endif //HTTPMGR_H
