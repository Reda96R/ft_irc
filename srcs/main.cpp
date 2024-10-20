#include "../includes/Client.hpp"
#include "../includes/macros.hpp"
#include "../includes/Server.hpp"



int main(int argc, char **argv)
{
    ServerInfo server_info;
    std::vector<pollfd> fds;

    if (!initialize_server(argc, argv, server_info))
        return (1);

    // Start listening for incoming connections
    listen(server_info.sockfd, 5);

    struct pollfd pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.fd = server_info.sockfd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd);

    while(true) {
        handlingPolling(server_info, fds, server_info.clients);
	}
    close(server_info.sockfd);
    return (0);

}


