#include "../../includes/Server.hpp"

void handlingPolling(ServerInfo& server_info, std::vector<pollfd>& fds, std::vector<Client*>& clients) {
    int poll_count = poll(fds.data(), fds.size(), -1);
        if (poll_count < 0) {
            std::cerr << RED << "Error polling" << RESET << std::endl;
            return;
        }
        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                 if (fds[i].fd == server_info.sockfd) {
                    Client *new_client = new Client();
                    if (!(*new_client).clientAdd(server_info.sockfd, clients, fds)) {
                        continue;
                    }
                } else {
                       if (!clients[i - 1]->clientRecv(server_info)) {
                        fds.erase(fds.begin() + i);
						delete clients[i - 1];
                        clients.erase(clients.begin() + i - 1);
                       }
                    
                    }
                }
        }
        
    }

