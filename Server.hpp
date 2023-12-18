#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <cstring>
#include <sys/epoll.h>
#include "ListeningSocket.hpp"

#define MAX_CLIENTS 20

using Clients = std::map<int, std::string>;

class Server
{
private:
    ListeningSocket _listening_socket;
    Clients _clients;
    int _epoll_fd;
    epoll_event _events[MAX_CLIENTS];
    int _port;

public:
    Server();
    Server(int port);
    ~Server();

    void run();

private:
    void addListeningSocketToEpoll();
    void handleNewConnection();
    void handleCloseConnection(int clientSocket);
    void handleReceivedData(int clientSocket, const char* buffer, int bytesReceived);
    void handleClientData(int clientSocket);
    void broadcastMessage(int senderSocket, const std::string& message);
};

#endif /* SERVER_HPP */
