#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in
# include <iostream>
# include <unistd.h> // For close()

class ListeningSocket
{
    private:
        int             _socket_fd;
        sockaddr_in     _sockaddr;
        int             _port;
    public:
        ListeningSocket();
        ListeningSocket(int);
        ~ListeningSocket();

        void    socket_init();
        void    binding();
        void    listening();
        int     accepting();
        void    closing();

        operator int() const;

};

#endif /*LISTENINGSOCKET_HPP*/