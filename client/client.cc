#include "client.h"
#include <iostream>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using proxy::SubscribeRequest;
using proxy::Message;

ClientImpl::ClientImpl(std::shared_ptr<Channel> channel) : stub_(proxy::Proxy::NewStub(channel)) {
}

void ClientImpl::Subscribe() {
    SubscribeRequest request;

    ClientContext context;
    std::unique_ptr<ClientReader<Message>> reader(stub_->Subscribe(&context, request));

    Message reply;
    while (reader->Read(&reply)) {
        std::cout << "Got reply: " << reply.data() << std::endl;
    }

    Status status = reader->Finish();
    if (status.ok()) {
        std::cout << "rpc succeeded." << std::endl;
    } else {
        std::cout << "rpc failed." << std::endl;
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
}
