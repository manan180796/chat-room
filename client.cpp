// Client side C/C++ program to demonstrate Socket programming
#include <iostream>
#include "Channel.hpp"
using namespace std;
#define PORT 8080


int main(int argc, char const *argv[]) {
    string serverAddress = std::string(argv[1]);
    int port = std::atoi(argv[2]);
    string hello = "Hello from client123456789012345678901234567890";
    Channel c(serverAddress, port);
    Messenger m(c);

    while (true) {
        getline(cin, hello);
        m.Send(hello);
        cout << "Server:  " << m.Read() << endl;
    }
    return 0;
}