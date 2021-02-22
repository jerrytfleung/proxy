#include "upstream.h"
#include <future>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using proxy::SubscribeRequest;
using proxy::Message;

Upstream::Upstream(std::shared_ptr<Channel> channel) : stub_(proxy::Proxy::NewStub(channel)) {
}

Upstream::~Upstream() {
    thread_.join();
}

void Upstream::Run() {
    thread_ = std::thread([this]() {
        SubscribeRequest request;
        ClientContext context;
        std::unique_ptr<ClientReader<Message>> reader(stub_->Subscribe(&context, request));
        Message reply;
        while (reader->Read(&reply)) {
            Notify(reply.data());
        }
        Status status = reader->Finish();
        if (status.ok()) {
            std::cout << "rpc succeeded." << std::endl;
        } else {
            std::cout << "rpc failed." << std::endl;
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
        }
    });
}