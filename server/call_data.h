#ifndef PROXY_CALL_DATA_H
#define PROXY_CALL_DATA_H

#include <mutex>
#include <queue>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>
#include <grpcpp/alarm.h>

#include "observer.h"
#include "generator.h"
#include "proxy.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::WriteOptions;
using proxy::SubscribeRequest;
using proxy::Message;

class CallData : public Observer<std::string> {
public:
    CallData(proxy::Proxy::AsyncService *service, ServerCompletionQueue *cq, Generator<std::string>* generator);

    void Proceed();

    void Cleanup();

    void Update(const std::string& s) override;

private:
    proxy::Proxy::AsyncService *service_;

    ServerCompletionQueue *cq_;

    ServerContext ctx_;

    SubscribeRequest request_;

    Message reply_;

    bool active_;

    ServerAsyncWriter<Message> responder_;

    enum class CallStatus {
        CREATE, PROCESS
    };

    CallStatus status_;

    std::queue<std::string> q_;

    Generator<std::string> *generator_;

    std::mutex mutex_;

    std::condition_variable cv_;
};

#endif //PROXY_CALL_DATA_H
