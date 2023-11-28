#ifndef SERVER_HPP
# define SERVER_HPP

// # include <cstdlib> // For exit() and EXIT_FAILURE
# include <iostream> // For cout
# include <unistd.h> // For read
# include <map>
# include <cstring> // For strlen
# include <sys/epoll.h>


# include "ListeningSocket.hpp"

# define MAX_CLIENTS = 20;

using Clients = std::map<int, std::string>;

class Server
{
    private:
        ListeningSocket     _listening_socket;
        Clients             _clients;
        int                 _epoll_fd;
        epoll_event         _events[20];
        int                 _port;
    public:
        Server();
        Server(int);
        ~Server();

        void    run();
};

#endif /*SERVER_HPP*/