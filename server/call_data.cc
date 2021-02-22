#include "call_data.h"
#include <iostream>
using namespace std;

CallData::CallData(proxy::Proxy::AsyncService *service, ServerCompletionQueue *cq, Generator<std::string> *generator)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CallStatus::CREATE), active_(false), generator_(generator) {
    Proceed();
}

void CallData::Proceed() {
    if (status_ == CallStatus::CREATE) {
        status_ = CallStatus::PROCESS;
        service_->RequestSubscribe(&ctx_, &request_, &responder_, cq_, cq_, this);
    } else if (status_ == CallStatus::PROCESS) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!active_) {
                // Create a call_data to handle next request
                new CallData(service_, cq_, generator_);
                // std::cout << "Next CallData: " << (void*)next_call_data << " created!" << std::endl;
                generator_->Attach(this);
                active_ = true;
            }
            cv_.wait(lock, [&]() {
                return !active_ || !q_.empty();
            });
            if (active_) {
                reply_.set_data(q_.front());
                q_.pop();
                responder_.Write(reply_, this);
            } else {
                // cout << "CallData: " << (void *) this << " become inactive!" << endl;
            }
        }
    } else {
        // cout << "Should not go here!" << endl;
    }
}

void CallData::Cleanup() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (active_) {
            generator_->Detach(this);
            active_ = false;
        }
    }
    // std::cout << "CallData: " << (void*)this << " deleted!" << std::endl;
    delete this;
}

void CallData::Update(const std::string& s) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (active_) {
        q_.push(s);
        cv_.notify_one();
    }
}