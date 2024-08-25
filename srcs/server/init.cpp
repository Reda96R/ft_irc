#include "../../includes/Server.hpp"

bool initialize_server(int argc, char **argv, ServerInfo &server_info)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
        return false;
    }

    char *endptr;
    long int port = std::strtol(argv[1], &endptr, 10);
    if (*endptr != '\0')
    {
        std::cerr << "Invalid argument" << std::endl;
        return false;
    }
    server_info.port = port;
    server_info.password = argv[2];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Socket creation failed\n";
        return false;
    }
	int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error setting SO_REUSEADDR" << std::endl;
        close(sockfd);
        return 1;
    }
    server_info.sockfd = sockfd;

    sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    server_info.serv_addr = serv_addr;
	server_info.servIpAddress = inet_ntoa(server_info.serv_addr.sin_addr);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
		perror("bind");
        std::cerr << "Binding failed\n";
        close(sockfd);
        return 1;
    }

    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "Error setting non-blocking mode.\n";
        close(sockfd);
        return 1;
    }

    return true;
}
