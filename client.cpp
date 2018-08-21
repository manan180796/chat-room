// Client side C/C++ program to demonstrate Socket programming
#include <pthread.h>
#include <iostream>
#include "Channel.hpp"
using namespace std;
#define PORT 8080

void* Reader(void* mess) {
    Messenger* messenger = (Messenger*)mess;
    while (true) {
        string m = messenger->Read();
        cout << endl;
        cout << m << endl;
        cout << ">> ";
    }
}

int main(int argc, char const* argv[]) {
    string serverAddress = std::string(argv[1]);
    int port = std::atoi(argv[2]);
    string hello = "Hello from client123456789012345678901234567890";
    Channel c(serverAddress, port);
    Messenger* m = new Messenger(c);

    pthread_t reader;
    pthread_create(&reader, NULL, Reader, (void*)m);

    while (true) {
        getline(cin, hello);
        m->Send(hello);
        cout << "Server:  " << m->Read() << endl;
    }
    return 0;
}