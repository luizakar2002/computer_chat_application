# include "ListeningSocket.hpp"

// TO DO: port update

ListeningSocket::ListeningSocket()
{
    // TO DO: check _socket_fd == -1 for exception
    _socket_fd = -1;
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = INADDR_ANY;
    _sockaddr.sin_port = htons(9998);
}

ListeningSocket::ListeningSocket(int port)
{
    std::cout << "ctor of ListeningSocket" << std::endl;
    // TO DO: check _socket_fd == -1 for exception
    _socket_fd = -1;
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = INADDR_ANY;
    _sockaddr.sin_port = htons(port);
}

ListeningSocket::~ListeningSocket()
{
}

void    ListeningSocket::socket_init()
{
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    binding();
    listening();
}

void    ListeningSocket::binding()
{
    if (bind(_socket_fd, (struct sockaddr*)&_sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cout << "Failed to bind to port " << _port << ". errno: " << errno << std::endl;
        // TO DO: exit() is not a good solution
        // exit(EXIT_FAILURE);
    }
}

void    ListeningSocket::listening()
{
    if (listen(_socket_fd, 20) < 0)
    {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        // TO DO: exit() is not a good solution
        // exit(EXIT_FAILURE);
    }
}

int    ListeningSocket::accepting()
{
    auto addrlen = sizeof(sockaddr);

    int connection = accept(_socket_fd, (struct sockaddr*)&_sockaddr, (socklen_t*)&addrlen);

    if (connection < 0)
    {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        // TO DO: exit() is not a good solution
        // exit(EXIT_FAILURE);
    }
    return connection;
}

void    ListeningSocket::closing()
{
    close(_socket_fd);
}

ListeningSocket::operator int() const {
    return _socket_fd;
}