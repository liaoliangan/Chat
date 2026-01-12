#ifndef VARIFYCLIENT_H
#define VARIFYCLIENT_H

#include "const.h"
#include "Singleton.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

class VarifyClient : public Singleton<VarifyClient>
{
    friend class Singleton<VarifyClient>;
public:
    message::GetVarifyRsp GetVarifyCode(std::string email);
    ~VarifyClient();
private:
    VarifyClient();
    //可以理解为连接的信使，只有通过stub才能和别人通信
    std::unique_ptr<message::VarifyService::Stub> stub;
};

#endif
