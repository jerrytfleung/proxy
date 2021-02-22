#ifndef PROXY_SYSTEMTIME_GENERATOR_H
#define PROXY_SYSTEMTIME_GENERATOR_H

#include <string>
#include <thread>
#include "generator.h"

class SystemTimeGenerator : public Generator<std::string> {
public:
    ~SystemTimeGenerator();

    void Run() override;
private:
    std::thread thread_;
};


#endif //PROXY_SYSTEMTIME_GENERATOR_H
