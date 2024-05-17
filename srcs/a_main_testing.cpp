#include "../includes/Client.hpp"
#include "../includes/macros.hpp"
#include "../includes/Server.hpp"



int main(int argc, char **argv)
{
    ServerInfo server_info;
    std::vector<Client> clients;
    std::vector<pollfd> fds;

    if (!initialize_server(argc, argv, server_info))
        return 1;

    // Start listening for incoming connections
    listen(server_info.sockfd, 5);

    struct pollfd pfd;
    pfd.fd = server_info.sockfd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd);

    while(true) {
        int poll_count = poll(fds.data(), fds.size(), -1);
        if (poll_count < 0) {
            std::cerr << RED << "Error polling" << RESET << std::endl;
            break;
        }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_info.sockfd) {
                    Client new_client;
                    if (!new_client.clientAdd(server_info.sockfd, clients))
                        continue;
                    struct pollfd pfd;
                    pfd.fd = new_client.getSocket();
                    pfd.events = POLLIN;
                    pfd.revents = 0;
                    fds.push_back(pfd);
                } else {
                    char buffer[1024] = {0};
                    int valread = recv(fds[i].fd, buffer, 1024, 0);
                    if (valread == 0) {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        clients.erase(clients.begin() + i);
                        --i;
                    } else if (valread == -1) {
                        std::cerr << RED << "Error reading from client" << RESET << std::endl;
                    } else {
                        buffer[valread] = 0;
                        std::cout << "Received: " << buffer << std::endl;
                        clients[i].clientRecv(buffer);

                }
            }
        }
        
    }

}
    close(server_info.sockfd);
    return 0;

}
