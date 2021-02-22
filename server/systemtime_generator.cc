#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
#include <iostream>
#include "systemtime_generator.h"

SystemTimeGenerator::~SystemTimeGenerator() {
    thread_.join();
}
void SystemTimeGenerator::Run() {
    thread_ = std::thread([this]() {
       while (true) {
           auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
           std::stringstream ss;
           ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
           Notify(ss.str());
           std::this_thread::sleep_for(std::chrono::seconds(1));
       }
    });
}