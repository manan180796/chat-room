// Server side C/C++ program to demonstrate Socket programming
#include <pthread.h>
#include <iostream>
#include <map>
#include <string>
#include "Channel.hpp"


using namespace std;
#define PORT 8080

vector<string> breaker(string statement) {
    char delimiter = ' ';
    vector<string> l;
    l.push_back("");
    for (int i = 0; i < statement.length(); ++i) {
        if (statement[i] == ' ')
            l.push_back("");
        else {
            l[l.size() - 1] += statement[i];
        }
    }
    return l;
}

void *client(void *sock) {
    int socket = *((int *)sock);
    User *user = new User(socket);
    user->Send("Please Enter Your Username");
    user->SetName(user->Get());
    ChatRoom *cr = nullptr;
    User::users[user->GetName()] = user;
    while (true) {
        while (cr == nullptr) {
            string response = user->Get();
            vector<string> l = breaker(response);
            if (response == "list chatrooms") {
                string message = "Here is a list of all the chatrooms\n";
                for (const auto &cr : ChatRoom::chatRooms) {
                    message += cr.first;
                }
                user->Send(message);
            } else if (l.size() == 3 && (l[0] == "create") &&
                       l[1] == "chatroom") {
                cr = new ChatRoom(l[2]);
                cr->AddUser(user);
                ChatRoom::chatRooms[l[2]] = cr;
            } else if (l.size() == 2 && l[0] == "join") {
                cr = ChatRoom::chatRooms[l[1]];
                cr->AddUser(user);
            } else if (l.size() == 1 && l[0] == "exit") {
                break;

            } else {
                user->Send("wrong input");
            }
        }
        if (cr == nullptr) break;
        while (cr != nullptr) {
            string response = user->Get();
            vector<string> l = breaker(response);
            if (l.size() == 1 && l[0] == "leave") {
                cr->RemoveUser(user);
                cr = nullptr;
            } else if (l.size() == 2 && l[0] == "reply") {
                cr->SendAll(l[1], user->GetName());
            } else {
                user->Send("wrong input");
            }
        }
    }
    delete user;
}

int main(int argc, char const *argv[]) {
    // string serverAddress = std::string(argv[1]);
    // int port = std::atoi(argv[2]);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int limit = 100, count = 0;
    string hello = "Hello from server";
    vector<int> occupied(limit, 0);
    vector<pthread_t> thread(limit);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while (true) {
        if (count >= limit) continue;
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        int i = 0;
        for (i = 0; i < occupied.size() && occupied[i] > 0; ++i)
            ;
        occupied[i] = new_socket;
        pthread_create(&thread[i], NULL, client, (void *)&occupied[i]);
        // Messenger m(new_socket);

        // while (true) {
        //     cout << "Client:  " << m.Read() << endl;
        //     getline(cin, hello);
        //     m.Send(hello);
        // }
    }
    return 0;
}
