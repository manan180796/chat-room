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
        cout << m << endl;
    }
}

int main(int argc, char const* argv[]) {
    string serverAddress = std::string(argv[1]);
    int port = std::atoi(argv[2]);
    Channel c(serverAddress, port);
    string input;
    Messenger* m = new Messenger(c);

    pthread_t reader;
    pthread_create(&reader, NULL, Reader, (void*)m);

    while (true) {
        // cin >> input;
        getline(cin, input);
        m->Send(input);
        cout << ">> ";
    }
    return 0;
}