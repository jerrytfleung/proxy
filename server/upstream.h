#ifndef PROXY_UPSTREAM_H
#define PROXY_UPSTREAM_H

#include <memory>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "proxy.grpc.pb.h"
#include "generator.h"

using grpc::Channel;

class Upstream : public Generator<std::string> {
public:
    Upstream(std::shared_ptr<Channel> channel);

    ~Upstream();

    void Run() override;
private:
    std::unique_ptr<proxy::Proxy::Stub> stub_;

    std::thread thread_;
};


#endif //PROXY_UPSTREAM_H
