/**
 * class UDPSocket
 * Description: A class for encuaplsating and abstracting the socket object
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP

#include "stdfax.h"
#include "message.hpp"

class UDPSocket {
private:
    int _sock;
    char _buffer[MAX_RECV];
    int _bytes;
    fd_set _fd;
    sockaddr_in _address;
    socklen_t _addressLength;
public:

    UDPSocket (int port = 0);
    virtual ~UDPSocket ();
    bool create(int port = 0);

    bool sendTo(UDPSocket& sock, std::string msg);
    bool sendMessageTo(UDPSocket& sock, Message message);
    int recvFrom(UDPSocket& sock, std::string &msg);
    int recvWithTimeout(UDPSocket& sock, std::string &msg);
    bool connect(UDPSocket &sock, std::string host, int port);

    bool isValid();
    int getPortNumber();
    std::string getHost();
    void updateSocketInfo(int port, std::string host);
};

#endif //UDPSOCKET_HPP
