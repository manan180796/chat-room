#ifndef Channel_hpp
#define Channel_hpp
#include <map>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Channel {
    std::string connectionMessage;

  public:
    std::string serverAddress;
    int port;
    struct sockaddr_in serv_addr;
    int sock;
    Channel(std::string serverAddress, int port);
    Channel(int socket);
    void Send(std::string message);
    std::string Read(int length);
    void Close();
};


class Messenger {
  private:
    Channel channel;
    int headerLength;

  public:
    Messenger(const Channel& channel);
    Messenger(int socket);
    void Send(std::string message);
    std::string Read();
    ~Messenger();
};


class User {
    Messenger messenger;
    std::string name;

  public:
    static std::map<std::string, User*> users;
    User(int socket);
    std::string GetName();
    void SetName(std::string name);
    void Send(std::string message);
    std::string Get();
};

class ChatRoom {
    std::vector<User*> users;
    std::string name;

  public:
    static std::map<std::string, ChatRoom*> chatRooms;
    void AddUser(User* user);
    std::string GetName();
    void SetName(std::string name);
    void SendAll(std::string message, std::string sender);
    std::vector<std::string> UserList();
};
#endif