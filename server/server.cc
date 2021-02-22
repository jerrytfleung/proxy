#include "server.h"
#include <utility>
#include "call_data.h"

ServerImpl::ServerImpl(Generator<std::string> *generator) {
    if (nullptr == generator) throw std::invalid_argument("generator cannot be nullptr!");
    generator_ = generator;
}

ServerImpl::~ServerImpl() {
    for (auto& th : threads_) {
        th.join();
    }
    server_->Shutdown();
    cq_->Shutdown();
}

void ServerImpl::Run() {
    std::string server_address("0.0.0.0:50052");

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    generator_->Run();
    // Spawn a new CallData instance to serve new clients
    new CallData(&service_, cq_.get(), generator_);

    for (int i = 0; i < SERVER_CONCURRENCY; i++) {
        threads_.push_back(std::thread([&]() {
            ServerImpl::HandleRpcs(cq_);
        }));
    }
}

void ServerImpl::HandleRpcs(std::unique_ptr<ServerCompletionQueue>& cq) {
    void *tag;
    bool ok;
    while (cq->Next(&tag, &ok)) {
        if (ok) {
            static_cast<CallData *>(tag)->Proceed();
        } else {
            static_cast<CallData *>(tag)->Cleanup();
        }
    }
}
