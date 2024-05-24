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
# include <poll.h>
#include <vector>
#include "Client.hpp"
#include "macros.hpp"

struct ServerInfo {
    int sockfd;
    int port;
    std::string password;
    sockaddr_in serv_addr;
    std::vector<Client*> clients;
};

bool initialize_server(int argc, char** argv, ServerInfo &server_info);
void handlingPolling(ServerInfo& server_info, std::vector<pollfd>& fds, std::vector<Client*>& clients);

#endif
