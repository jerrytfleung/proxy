#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "proxy.grpc.pb.h"
#include "generator.h"

using grpc::Server;
using grpc::ServerCompletionQueue;

class ServerImpl {
public:
    constexpr static int SERVER_CONCURRENCY = 4;

    ServerImpl(Generator<std::string>* generator);

    ~ServerImpl();

    void Run();

private:
    static void HandleRpcs(std::unique_ptr<ServerCompletionQueue>& cq);

    std::unique_ptr<ServerCompletionQueue> cq_;

    proxy::Proxy::AsyncService service_;

    std::unique_ptr<Server> server_;

    Generator<std::string>* generator_;

    std::vector<std::thread> threads_;
};

#endif //PROXY_SERVER_H
