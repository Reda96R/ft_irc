#ifndef _SERVER_HPP_
# define _SERVER_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
# include <netinet/in.h>

struct ServerInfo {
    int sockfd;
    int port;
    std::string password;
    sockaddr_in serv_addr;
};

bool initialize_server(int argc, char** argv, ServerInfo &server_info);

#endif