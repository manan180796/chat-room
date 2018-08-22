#ifndef Channel_cpp
#define Channel_cpp
#include "Channel.hpp"
#include <iostream>
#include <map>
#define messageBlock 1024


std::map<std::string, User*> User::users;
std::map<std::string, ChatRoom*> ChatRoom::chatRooms;

Channel::Channel(std::string serverAddress, int port)
    : serverAddress(serverAddress), port(port), sock(0) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        connectionMessage = "Socket creation error";
        return;
    }
    // memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, serverAddress.c_str(), &serv_addr.sin_addr) <= 0) {
        connectionMessage = "Invalid address / Address not supported";
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        connectionMessage = "Connection Failed";
        return;
    }
}
Channel::Channel(int socket) : sock(socket) {}
void Channel::Send(std::string message) {
    send(sock, message.c_str(), message.length(), 0);
}
std::string Channel::Read(int length) {
    char buffer[messageBlock] = {0};

    int valread = read(sock, buffer, length);
    return std::string(buffer);
}

void Channel::Close() { close(sock); }


Messenger::Messenger(const Channel& channel)
    : channel(channel), headerLength(8) {}
Messenger::Messenger(int socket) : channel(socket), headerLength(8) {}
Messenger::~Messenger() { channel.Close(); }

void Messenger::Send(std::string message) {
    std::string header = std::to_string(message.length());
    while (header.length() < headerLength) header.insert(header.begin(), '0');
    channel.Send(header);
    channel.Send(message);
}

std::string Messenger::Read() {
    std::string length = channel.Read(headerLength);
    int len = std::atoi(length.c_str());
    std::string added;
    std::string message = "";
    while (len > 0) {
        if (len > messageBlock) {
            added = channel.Read(messageBlock);
            len -= added.length();
            message += added;
        } else {
            added = channel.Read(len);
            len -= added.length();
            message += added;
        }
    }
    return message;
}


ChatRoom::ChatRoom(std::string name) : name(name) {}
void ChatRoom::AddUser(User* user) {
    SendAll("Welcome " + user->GetName() + "\n", "");
    users.push_back(user);
    user->Send("Welcome to " + name);
    std::cout << "sent to" << user->GetName() << std::endl;
}

void ChatRoom::RemoveUser(User* user) {
    int pos = 0;
    for (pos = 0; pos < users.size(); ++pos) {
        if (users[pos]->GetName() == user->GetName()) {
            users.erase(users.begin() + pos);
            break;
        }
    }
    SendAll(user->GetName() + " exited\n", "");
    user->Send("You exited from " + name);
    std::cout << "sent to" << user->GetName() << std::endl;
}

std::string ChatRoom::GetName() { return name; }
void ChatRoom::SendAll(std::string message, std::string sender) {
    for (int i = 0; i < users.size(); ++i) {
        if (users[i]->GetName() != sender) {
            users[i]->Send(message);
        }
    }
}

void ChatRoom::SetName(std::string name) { this->name = name; }

User::User(int socket) : messenger(socket) {}
std::string User::GetName() { return name; }
void User::Send(std::string message) { messenger.Send(message); }
void User::SetName(std::string name) { this->name = name; }
std::string User::Get() { return messenger.Read(); }

#endif