//
// Created by 33717 on 2026/1/17.
//

#ifndef TCPMGR_H
#define TCPMGR_H
#include<QTcpSocket>
#include"singalton.h"
#include"global.h"
#include<functional>
#include<QObject>

#include "AuthRsp.h"
#include "ServerInfo.h"
#include "SearchInfo.h"

class TcpMgr : public QObject,
               public Singalton<TcpMgr>,
               public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
    friend class Singalton;

public:
    ~TcpMgr() override;

private:
    TcpMgr();
    void handleMsg(LA::ReqId id, int len, QByteArray data);
    void initHandlers();
    QTcpSocket _socket;
    QString _host;
    quint16 _port;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
    QMap<LA::ReqId, std::function<void(LA::ReqId, int len, QByteArray data)>> _handlers;

public slots:
    void slot_tcp_connect(const ServerInfo&);
    void slot_send_data(LA::ReqId, QByteArray data);
signals:
    void sig_con_success(bool success);
    void sig_send_data(LA::ReqId, QByteArray data);
    void sig_switch_chatdlg();
    void sig_login_failed(int);
    void sig_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
    void sig_user_search(std::shared_ptr<SearchInfo>);
};


#endif //TCPMGR_H
