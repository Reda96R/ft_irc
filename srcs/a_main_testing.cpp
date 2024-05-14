#include "../includes/Client.hpp"
#include "../includes/macros.hpp"
#include "../includes/Server.hpp"



int main(int argc, char **argv)
{
    ServerInfo server_info;

    if (!initialize_server(argc, argv, server_info))
        return 1;

    // Start listening for incoming connections
    listen(server_info.sockfd, 5);

    while(true) {
        // Pass server_info to the functions that handle connections, authentication, etc.
    }

    close(server_info.sockfd);
    return 0;
}
