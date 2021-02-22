#include <iostream>
#include "client.h"

using namespace std;

int main(int argc, char **argv) {
    cout << "Start client..." << endl;
    ClientImpl client(grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials()));
    client.Subscribe();
    return 0;
}