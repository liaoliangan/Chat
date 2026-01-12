#ifndef VARIFYCLIENT_H
#define VARIFYCLIENT_H

#include "const.h"
#include "Singleton.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "RPConPool.h"

class VarifyClient : public Singleton<VarifyClient>
{
    friend class Singleton<VarifyClient>;

public:
    message::GetVarifyRsp GetVarifyCode(std::string email);
    ~VarifyClient();

private:
    VarifyClient();
    std::unique_ptr<RPConPool> pool_;
};

#endif
