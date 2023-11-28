# include "Server.hpp"

Server::Server()
{
    _listening_socket.socket_init();
    _epoll_fd = epoll_create1(0);

    // TO DO
    if (_epoll_fd == -1) {
        std::cerr << "Error creating epoll" << std::endl;
    }
}

Server::Server(int port): _listening_socket(port)
{
    _listening_socket.socket_init();
    _epoll_fd = epoll_create1(0);

    // TO DO
    if (_epoll_fd == -1) {
        std::cerr << "Error creating epoll" << std::endl;
    }
}

Server::~Server()
{
    close(_epoll_fd);
    _listening_socket.closing();
}

void    Server::run()
{
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = _listening_socket;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _listening_socket, &event) == -1)
    {
        std::cerr << "Error adding server socket to epoll" << std::endl;
    }

    while (42)
    {
        int numEvents = epoll_wait(_epoll_fd, _events, 20, -1);
        if (numEvents == -1)
        {
            std::cerr << "Error in epoll_wait" << std::endl;
            break;
        }
        for (int i = 0; i < numEvents; ++i)
        {
            if (_events[i].data.fd == _listening_socket)
            {
                int clientSocket = _listening_socket.accepting();

                event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
                event.data.fd = clientSocket;
                epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, clientSocket, &event);

                _clients[clientSocket] = std::to_string(clientSocket);
            }
            else
            {
                char buffer[1024];
                int clientSocket = _events[i].data.fd;
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
                std::string received_string(buffer, bytesReceived);

                std::cout << "Received: " << received_string << std::endl;

                if (bytesReceived <= 0)
                {
                    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, clientSocket, NULL);
                    close(clientSocket);
                    for (auto it = _clients.begin(); it != _clients.end(); ++it)
                    {
                        if (it->first == _listening_socket)
                        {
                            _clients.erase(it);
                            break;
                        }
                    }
                }
                else
                {
                    if (received_string.substr(0, 10) == "Nickname: ")
                    {
                        _clients[clientSocket] = received_string.substr(10);
                    }
                    else
                    {
                        received_string = _clients[clientSocket] + ": " + received_string;
                        for (const auto& pair : _clients)
                        {
                            if (pair.first != clientSocket)
                            {
                                send(pair.first, received_string.c_str(), received_string.size(), 0);
                            }
                        }
                    }
                }
            }
        }
    }
}