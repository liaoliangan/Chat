//
// Created by 33717 on 2026/1/17.
//
#include<QAbstractSocket>
#include "TcpMgr.h"
#include <QJsonDocument>
#include <utility>
#include"UserMgr.h"

TcpMgr::~TcpMgr()
{
}

TcpMgr::TcpMgr(): _host(""), _port(0), _b_recv_pending(false), _message_id(0), _message_len(0)
{
    connect(&_socket, &QTcpSocket::connected, [&]()
    {
        qDebug() << "QTcpSocket: Connected to server!";
        // 连接建立后发送消息
        emit sig_con_success(true);
    });

    connect(&_socket, &QTcpSocket::readyRead, [&]() //收到回包
    {
        // 当有数据可读时，读取所有数据
        // 读取所有数据并追加到缓冲区
        _buffer.append(_socket.readAll());

        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_6_0);

        forever
        {
            //先解析头部
            if (!_b_recv_pending)
            {
                // 检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度），ID占两个字节，长度占两个字节
                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2))
                {
                    return; // 数据不够，等待更多数据
                }

                // 预读取消息ID和消息长度，但不从缓冲区中移除
                stream >> _message_id >> _message_len;

                //将buffer 中的前四个字节移除
                _buffer = _buffer.mid(sizeof(quint16) * 2);

                // 输出读取的数据
                qDebug() << "Message ID:" << _message_id << ", Length:" << _message_len;
            }

            //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
            if (_buffer.size() < _message_len)
            {
                _b_recv_pending = true;
                return;
            }

            _b_recv_pending = false;
            // 读取消息体
            QByteArray messageBody = _buffer.mid(0, _message_len);
            qDebug() << "receive body msg is " << messageBody;

            _buffer = _buffer.mid(_message_len);
            handleMsg(static_cast<LA::ReqId>(_message_id), _message_len, messageBody);
        }
    });

    //5.15 之后版本
    connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            [&](QAbstractSocket::SocketError socketError)
            {
                // Q_UNUSED(socketError)
                // qDebug() << "Error:" << _socket.errorString();
                qDebug() << "Error:" << _socket.errorString();
                switch (socketError)
                {
                case QTcpSocket::ConnectionRefusedError:
                    qDebug() << "Connection Refused!";
                    emit sig_con_success(false);
                    break;
                case QTcpSocket::RemoteHostClosedError:
                    qDebug() << "Remote Host Closed Connection!";
                    break;
                case QTcpSocket::HostNotFoundError:
                    qDebug() << "Host Not Found!";
                    emit sig_con_success(false);
                    break;
                case QTcpSocket::SocketTimeoutError:
                    qDebug() << "Connection Timeout!";
                    emit sig_con_success(false);
                    break;
                case QTcpSocket::NetworkError:
                    qDebug() << "Network Error!";
                    break;
                default:
                    qDebug() << "Other Error!";
                    break;
                }
            });

    // 处理连接断开
    connect(&_socket, &QTcpSocket::disconnected, [&]()
    {
        qDebug() << "Disconnected from server.";
    });

    connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    //注册消息
    initHandlers();
}

void TcpMgr::handleMsg(LA::ReqId id, int len, QByteArray data)
{
    qDebug() << "handleMsg: handle id is " << static_cast<int>(id) << " data is " << data;
    auto find_iter = _handlers.find(id);
    if (find_iter == _handlers.end())
    {
        qDebug() << "not found id [" << static_cast<int>(id) << "] to handle";
        return;
    }
    find_iter.value()(id, len, std::move(data));
}

void TcpMgr::initHandlers()
{
    _handlers.insert(LA::ReqId::ID_CHAT_LOGIN_RSP, [this](LA::ReqId id, const int len, const QByteArray& data)
    {
        Q_UNUSED(len);
        qDebug() << "handle id is " << "ID_CHAT_LOGIN_RSP" << " data is " << data;
        const QJsonDocument doc = QJsonDocument::fromJson(data);

        //检查转换是否成功
        if (doc.isNull())
        {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }
        QJsonObject jsonObj = doc.object();
        if (!jsonObj.contains("error"))
        {
            constexpr int err = static_cast<int>(LA::ErrorCodes::ERROR_JSON);
            qDebug() << "Login failed,err is Json Parse Err" << err;
            emit sig_login_failed(err);
            return;
        }

        if (const int err = jsonObj["error"].toInt(); err != static_cast<int>(LA::ErrorCodes::SUCCESS))
        {
            qDebug() << "Login failed,err is " << err;
            emit sig_login_failed(err);
            return;
        }

        UserMgr::getInstance()->SetUserInfo(
            std::make_shared<UserInfo>(jsonObj["uid"].toInt(), jsonObj["name"].toString(), "", "", 0));
        UserMgr::getInstance()->SetToken(jsonObj["token"].toString());
        emit sig_switch_chatdlg();
    });
}

void TcpMgr::slot_tcp_connect(const ServerInfo& si)
{
    qDebug() << "receive tcp connect signal";
    //连接到服务器
    qDebug() << "Connecting to server...";
    qDebug() << "host is " << si.Host << " port is " << si.Port;
    _host = si.Host;
    // _port = si.Port.toUShort();
    _port = 8090;
    _socket.connectToHost(_host, _port);
}

void TcpMgr::slot_send_data(LA::ReqId reqId, QString data)
{
    const quint16 id = static_cast<quint16>(reqId);
    const QByteArray dataBytes = data.toUtf8();
    const quint16 len = static_cast<quint16>(data.size());
    //创建一个QByteArray用来存储发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);
    //写入ID和长度
    out << id << len;
    //添加字符串数据
    block.append(dataBytes);
    //发送数据
    _socket.write(block);
}
