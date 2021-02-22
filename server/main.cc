#include <iostream>
#include "server.h"
#include "systemtime_generator.h"
#include "upstream.h"
using namespace std;

int main(int argc, char **argv) {
    cout << "Start server..." << endl;
    SystemTimeGenerator systemtime_generator;
    Upstream upstream(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    ServerImpl impl(&systemtime_generator);
    impl.Run();
    return 0;
}