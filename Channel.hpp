#ifndef Channel_hpp
#define Channel_hpp
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


class Client {
    Messenger messenger;
    std::string name;
};

class ChatRoom {
    std::vector<Client> clients;
    std::string name;
};
#endif