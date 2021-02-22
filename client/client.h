#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include <memory>
#include <grpcpp/grpcpp.h>
#include "proxy.grpc.pb.h"

using grpc::Channel;

class ClientImpl {
public:
    ClientImpl(std::shared_ptr<Channel> channel);

    void Subscribe();

private:
    std::unique_ptr<proxy::Proxy::Stub> stub_;
};


#endif //PROXY_CLIENT_H
