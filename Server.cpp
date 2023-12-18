#include "Server.hpp"

Server::Server() : _epoll_fd(epoll_create1(0))
{
    if (_epoll_fd == -1)
    {
        std::cerr << "Error creating epoll" << std::endl;
    }
    else
    {
        _listening_socket.socket_init();
        addListeningSocketToEpoll();
    }
}

Server::Server(int port) : _listening_socket(port), _epoll_fd(epoll_create1(0))
{
    if (_epoll_fd == -1)
    {
        std::cerr << "Error creating epoll" << std::endl;
    }
    else
    {
        _listening_socket.socket_init();
        addListeningSocketToEpoll();
    }
}

Server::~Server()
{
    close(_epoll_fd);
    _listening_socket.closing();
}

void Server::run()
{
    while (true)
    {
        int numEvents = epoll_wait(_epoll_fd, _events, MAX_CLIENTS, -1);
        if (numEvents == -1)
        {
            std::cerr << "Error in epoll_wait" << std::endl;
            break;
        }

        for (int i = 0; i < numEvents; ++i)
        {
            if (_events[i].data.fd == _listening_socket)
            {
                handleNewConnection();
            }
            else
            {
                handleClientData(_events[i].data.fd);
            }
        }
    }
}

void Server::addListeningSocketToEpoll()
{
    epoll_event event;
    event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
    event.data.fd = _listening_socket;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _listening_socket, &event) == -1)
        std::cerr << "Error adding server socket to epoll" << std::endl;
}

void Server::handleNewConnection()
{
    int clientSocket = _listening_socket.accepting();

    epoll_event event;
    event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
    event.data.fd = clientSocket;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1)
    {
        std::cerr << "Error adding client socket to epoll" << std::endl;
        close(clientSocket);
        return;
    }

    _clients[clientSocket] = std::to_string(clientSocket);
}

void Server::handleClientData(int clientSocket)
{
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived <= 0)
        handleCloseConnection(clientSocket);
    else
        handleReceivedData(clientSocket, buffer, bytesReceived);
}

void Server::handleCloseConnection(int clientSocket)
{
    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, clientSocket, NULL);
    close(clientSocket);

    auto it = _clients.find(clientSocket);
    if (it != _clients.end())
    {
        _clients.erase(it);
    }
}

void Server::handleReceivedData(int clientSocket, const char* buffer, int bytesReceived)
{
    std::string receivedString(buffer, bytesReceived);

    std::cout << "Received: " << receivedString << std::endl;

    if (receivedString.substr(0, 10) == "Nickname: ")
        _clients[clientSocket] = receivedString.substr(10);
    else
    {
        receivedString = _clients[clientSocket] + ": " + receivedString;
        broadcastMessage(clientSocket, receivedString);
    }
}

void Server::broadcastMessage(int senderSocket, const std::string& message)
{
    for (const auto& pair : _clients)
        if (pair.first != senderSocket)
            send(pair.first, message.c_str(), message.size(), 0);
}
